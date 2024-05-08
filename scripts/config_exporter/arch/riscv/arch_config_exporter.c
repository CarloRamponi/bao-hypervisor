/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <arch_config_exporter.h>
#include <ccan/json/json.h>
#include <json_typed.h>

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