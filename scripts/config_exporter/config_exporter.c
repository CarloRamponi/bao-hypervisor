/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <config_exporter.h>
#include <stdio.h>

extern struct config config;

struct JsonNode *spinlock_to_json(spinlock_t *lock) {
    struct JsonNode *json = json_mkobject();
    json_append_member(json, "ticket", json_typed_number("uint32_t", lock->ticket));
    json_append_member(json, "next", json_typed_number("uint32_t", lock->next));
    return json;
}

struct JsonNode *shmem_to_json(struct shmem *shmem) {
    struct JsonNode *json = json_mkobject();
    json_append_member(json, "size", json_typed_number("size_t", shmem->size));
    json_append_member(json, "colors", json_typed_number("colormap_t", shmem->colors));
    json_append_member(json, "place_phys", json_typed_boolean("bool", shmem->place_phys));
    json_append_member(json, "base", json_typed_number("paddr_t", shmem->base));
    json_append_member(json, "phys", json_typed_number("paddr_t", shmem->phys));
    json_append_member(json, "cpu_masters", json_typed_number("cpumap_t", shmem->cpu_masters));
    json_append_member(json, "lock", json_typed_object("spinlock_t", spinlock_to_json(&shmem->lock)));
    return json;
}

struct JsonNode *vm_mem_region_to_json(struct vm_mem_region *region) {
    struct JsonNode *json = json_mkobject();
    json_append_member(json, "base", json_typed_number("paddr_t", region->base));
    json_append_member(json, "size", json_typed_number("size_t", region->size));
    json_append_member(json, "colors", json_typed_number("colormap_t", region->colors));
    json_append_member(json, "place_phys", json_typed_boolean("bool", region->place_phys));
    json_append_member(json, "phys", json_typed_number("paddr_t", region->phys));
    return json;
}

struct JsonNode *ipc_to_json(struct ipc *ipc) {
    struct JsonNode *json = json_mkobject();
    json_append_member(json, "base", json_typed_number("paddr_t", ipc->base));
    json_append_member(json, "size", json_typed_number("size_t", ipc->size));
    json_append_member(json, "shmem_id", json_typed_number("int", ipc->shmem_id));
    json_append_member(json, "interrupt_num", json_typed_number("int", ipc->interrupt_num));
    struct JsonNode *interrupts = json_mkarray();
    for (int i = 0; i < ipc->interrupt_num; i++) {
        json_append_element(interrupts, json_typed_number("irqid_t", ipc->interrupts[i]));
    }
    json_append_member(json, "interrupts", interrupts);
    return json;
}

struct JsonNode *vm_dev_region_to_json(struct vm_dev_region *region) {
    struct JsonNode *json = json_mkobject();
    json_append_member(json, "pa", json_typed_number("paddr_t", region->pa));
    json_append_member(json, "va", json_typed_number("vaddr_t", region->va));
    json_append_member(json, "size", json_typed_number("size_t", region->size));
    json_append_member(json, "interrupt_num", json_typed_number("int", region->interrupt_num));
    struct JsonNode *interrupts = json_mkarray();
    for (int i = 0; i < region->interrupt_num; i++) {
        json_append_element(interrupts, json_typed_number("irqid_t", region->interrupts[i]));
    }
    json_append_member(json, "interrupts", interrupts);
    json_append_member(json, "id", json_typed_number("int", region->id));
    return json;
}

struct JsonNode *arch_vm_platform_to_json(struct arch_vm_platform *platform) {
    struct JsonNode *json = json_mkobject();
    struct JsonNode *irqc = json_mkobject();
    struct JsonNode *plic = json_mkobject();
    json_append_member(plic, "base", json_typed_number("paddr_t", platform->irqc.plic.base));
    json_append_member(irqc, "plic", json_typed_object("anonymous struct", plic));

    struct JsonNode *aia = json_mkobject();
    struct JsonNode *aplic = json_mkobject();
    json_append_member(aplic, "base", json_typed_number("paddr_t", platform->irqc.aia.aplic.base));
    json_append_member(aia, "aplic", json_typed_object("anonymous struct", aplic));
    json_append_member(irqc, "aia", json_typed_object("anonymous struct", aia));
    json_append_member(json, "irqc", json_typed_object("vm_irqc_dscrp", irqc));
    return json;
}

struct JsonNode *vm_platform_to_json(struct vm_platform *platform) {
    struct JsonNode *json = json_mkobject();
    json_append_member(json, "cpu_num", json_typed_number("size_t", platform->cpu_num));
    json_append_member(json, "region_num", json_typed_number("size_t", platform->region_num));
    struct JsonNode *regions = json_mkarray();
    for (int i = 0; i < platform->region_num; i++) {
        json_append_element(
            regions,
            json_typed_object(
                "vm_mem_region",
                vm_mem_region_to_json(&platform->regions[i])
            )
        );
    }
    json_append_member(json, "regions", regions);

    json_append_member(json, "ipc_num", json_typed_number("size_t", platform->ipc_num));
    struct JsonNode *ipcs = json_mkarray();
    for (int i = 0; i < platform->ipc_num; i++) {
        json_append_element(
            ipcs,
            json_typed_object(
                "ipc",
                ipc_to_json(&platform->ipcs[i])
            )
        );
    }
    json_append_member(json, "ipcs", ipcs);

    json_append_member(json, "dev_num", json_typed_number("size_t", platform->dev_num));
    struct JsonNode *devs = json_mkarray();
    for (int i = 0; i < platform->dev_num; i++) {
        json_append_element(
            devs,
            json_typed_object(
                "vm_dev_region",
                vm_dev_region_to_json(&platform->devs[i])
            )
        );
    }
    json_append_member(json, "devs", devs);
    json_append_member(json, "mmu", json_typed_boolean("bool", platform->mmu));
    json_append_member(
        json,
        "arch",
        json_typed_object(
            "arch_vm_platform",
            arch_vm_platform_to_json(&platform->arch)
        )
    );
    return json;
}

struct JsonNode *vm_config_to_json(struct vm_config *vm_config) {
    struct JsonNode *json = json_mkobject();

    struct JsonNode *image = json_mkobject();
    json_append_member(image, "base_addr", json_typed_number("vaddr_t", vm_config->image.base_addr));
    json_append_member(image, "load_addr", json_typed_number("paddr_t", vm_config->image.load_addr));
    json_append_member(image, "size", json_typed_number("size_t", vm_config->image.size));
    json_append_member(json, "image", json_typed_object("anonymous struct", image));

    json_append_member(json, "entry", json_typed_number("vaddr_t", vm_config->entry));
    json_append_member(json, "cpu_affinity", json_typed_number("cpumap_t", vm_config->cpu_affinity));
    json_append_member(json, "colors", json_typed_number("colormap_t", vm_config->colors));

    json_append_member(json, "platform", json_typed_object("vm_platform", vm_platform_to_json(&vm_config->platform)));
    return json;
}

struct JsonNode *config_to_json(struct config *config) {
    struct JsonNode *json = json_mkobject();

    struct JsonNode *hyp = json_mkobject();
    json_append_member(hyp, "relocate", json_typed_boolean("bool", config->hyp.relocate));
    json_append_member(hyp, "base_addr", json_typed_number("paddr_t", config->hyp.base_addr));
    json_append_member(hyp, "colors", json_typed_number("colormap_t", config->hyp.colors));
    json_append_member(json, "hyp", json_typed_object("anonymous struct", hyp));

    json_append_member(json, "shmemlist_size", json_typed_number("size_t", config->shmemlist_size));
    struct JsonNode *shmemlist = json_mkarray();
    for (int i = 0; i < config->shmemlist_size; i++) {
        json_append_element(shmemlist, json_typed_object("shmem", shmem_to_json(&config->shmemlist[i])));
    }
    json_append_member(json, "shmemlist", shmemlist);

    json_append_member(json, "vmlist_size", json_typed_number("size_t", config->vmlist_size));
    struct JsonNode *vmlist = json_mkarray();
    for (int i = 0; i < config->vmlist_size; i++) {
        json_append_element(vmlist, json_typed_object("vm_config", vm_config_to_json(&config->vmlist[i])));
    }
    json_append_member(json, "vmlist", vmlist);
    return json;
}

void dump_config(struct config *config, FILE *file) {
    struct JsonNode *json = json_typed_object("config", config_to_json(config));
    const char *json_str = json_stringify(json, "\t");
    fprintf(file, "%s\n", json_str);
}

int main(int argc, char **argv) {

    FILE *file = stdout;

    if(argc > 1) {

        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            fprintf(stderr, "Usage: %s [output_file]\n", argv[0]);
            return 0;
        }

        file = fopen(argv[1], "w");
        if(file == NULL) {
            fprintf(stderr, "Error opening file %s\n", argv[1]);
            return 1;
        }
    }

    dump_config(&config, file);
    return 0;
}