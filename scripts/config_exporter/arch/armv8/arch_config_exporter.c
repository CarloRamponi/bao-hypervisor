/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <arch_config_exporter.h>
#include <ccan/json/json.h>
#include <json_typed.h>

struct JsonNode *arch_vm_platform_to_json(struct arch_vm_platform *platform) {
    struct JsonNode *json = json_mkobject();
    struct JsonNode *gic = json_mkobject();

    json_append_member(gic, "gicd_addr", json_typed_number("paddrt_t", platform->gic.gicd_addr));
    json_append_member(gic, "gicc_addr", json_typed_number("paddrt_t", platform->gic.gicc_addr));
    json_append_member(gic, "gicr_addr", json_typed_number("paddrt_t", platform->gic.gicr_addr));
    json_append_member(gic, "interrupt_num", json_typed_number("paddrt_t", platform->gic.interrupt_num));
    json_append_member(json, "gic", json_typed_object("vgic_dscrp", gic));

#ifdef MEM_PROT_MMU
    struct JsonNode *smmu = json_mkobject();
    json_append_member(smmu, "global_mask", json_typed_number("streamid_t", platform->smmu.global_mask));
    json_append_member(smmu, "group_num", json_typed_number("size_t", platform->smmu.group_num));

    struct JsonNode *groups = json_mkarray();
    for (int i = 0; i < platform->smmu.group_num; i++) {
        struct JsonNode *group = json_mkobject();
        json_append_member(group, "mask", json_typed_number("streamid_t", platform->smmu.groups[i].mask));
        json_append_member(group, "id", json_typed_number("streamid_t", platform->smmu.groups[i].id));
        json_append_element(groups, group);
    }
    json_append_member(smmu, "groups", json_typed_object("smmu_group *", groups));
    json_append_member(json, "smmu", json_typed_object("anonymous struct", smmu));
#endif
    return json;
}