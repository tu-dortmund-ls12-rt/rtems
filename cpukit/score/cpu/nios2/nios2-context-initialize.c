/*
 * Copyright (c) 2011, 2021 embedded brains GmbH
 *
 * Copyright (c) 2006 Kolja Waschk (rtemsdev/ixo.de)
 *
 * COPYRIGHT (c) 1989-2006
 * On-Line Applications Research Corporation (OAR).
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include <rtems/score/cpu.h>
#include <rtems/score/nios2-utility.h>
#include <rtems/score/interr.h>
#include <rtems/score/tls.h>

void _CPU_Context_Initialize(
  Context_Control *context,
  void *stack_area_begin,
  size_t stack_area_size,
  uint32_t new_level,
  void (*entry_point)( void ),
  bool is_fp,
  void *tls_area
)
{
  const Nios2_MPU_Configuration *mpu_config = _Nios2_MPU_Get_configuration();
  uint32_t stack = (uint32_t) stack_area_begin + stack_area_size - 4;

  memset(context, 0, sizeof(*context));

  context->fp = stack;
  context->status = _Nios2_ISR_Set_level( new_level, NIOS2_STATUS_PIE );
  context->sp = stack;
  context->ra = (uint32_t) entry_point;

  if ( mpu_config != NULL ) {
    Nios2_MPU_Region_descriptor desc = {
      .index = mpu_config->data_index_for_stack_protection,
      .base = stack_area_begin,
      .end = (const void *) RTEMS_ALIGN_UP(
        (uintptr_t) stack_area_begin + stack_area_size +
          _TLS_Get_allocation_size(),
        1U << mpu_config->data_region_size_log2
      ),
      .perm = NIOS2_MPU_DATA_PERM_SVR_READWRITE_USER_NONE,
      .data = true,
      .cacheable = mpu_config->enable_data_cache_for_stack,
      .read = false,
      .write = true
    };
    bool ok = _Nios2_MPU_Setup_region_registers(
      mpu_config,
      &desc,
      &context->stack_mpubase,
      &context->stack_mpuacc
    );

    if ( !ok ) {
      /* The task stack allocator must ensure that the stack area is valid */
      _Terminate( INTERNAL_ERROR_CORE, 0xdeadbeef );
    }
  }

  if ( tls_area != NULL ) {
    context->r23 = (uintptr_t) tls_area +
      _TLS_Get_thread_control_block_area_size( (uintptr_t) _TLS_Alignment ) +
       0x7000;
    _TLS_TCB_before_TLS_block_initialize( tls_area );
  }
}
