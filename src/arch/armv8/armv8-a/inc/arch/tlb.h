/**
 * SPDX-License-Identifier: GPL-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __ARCH_TLB_H__
#define __ARCH_TLB_H__

#include <bao.h>
#include <arch/sysregs.h>
#include <arch/fences.h>

static inline void tlb_hyp_inv_va(vaddr_t va)
{
    asm volatile(
        "dsb  ish\n\t"
        "tlbi vae2is, %0\n\t"
        "dsb  ish\n\t"
        "isb\n\t" ::"r"(va >> 12));
}

static inline void tlb_hyp_inv_all()
{
    asm volatile(
        "dsb  ish\n\t"
        "tlbi alle2is\n\t"
        "dsb  ish\n\t"
        "isb\n\t");
}

static inline void tlb_vm_inv_va(asid_t vmid, vaddr_t va)
{
    uint64_t vttbr = 0;
    vttbr = sysreg_vttbr_el2_read();
    bool switch_vmid =
        bit64_extract(vttbr, VTTBR_VMID_OFF, VTTBR_VMID_LEN) != vmid;

    if (switch_vmid) {
        sysreg_vttbr_el2_write(((vmid << VTTBR_VMID_OFF) & VTTBR_VMID_MSK));
        DSB(ish);
        ISB();
    }

    asm volatile("tlbi ipas2e1is, %0\n\t" ::"r"(va >> 12));

    if (switch_vmid) {
        DSB(ish);
        sysreg_vttbr_el2_write(((vmid << VTTBR_VMID_OFF) & VTTBR_VMID_MSK));
    }
}

static inline void tlb_vm_inv_all(asid_t vmid)
{
    uint64_t vttbr = 0;
    vttbr = sysreg_vttbr_el2_read();
    bool switch_vmid =
        bit64_extract(vttbr, VTTBR_VMID_OFF, VTTBR_VMID_LEN) != vmid;

    if (switch_vmid) {
        sysreg_vttbr_el2_write(((vmid << VTTBR_VMID_OFF) & VTTBR_VMID_MSK));
        DSB(ish);
        ISB();
    }

    asm volatile("tlbi vmalls12e1is\n\t");

    if (switch_vmid) {
        DSB(ish);
        sysreg_vttbr_el2_write(((vmid << VTTBR_VMID_OFF) & VTTBR_VMID_MSK));
    }
}

#endif /* __ARCH_TLB_H__ */
