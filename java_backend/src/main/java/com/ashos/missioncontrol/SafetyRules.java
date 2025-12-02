package com.ashos.missioncontrol;

import java.util.Map;

public class SafetyRules {
    public boolean validate(Map<String, Object> cmd) {
        try {
            double o2 = getNumber(cmd, "set_o2_percent");
            if (o2 < 1800 || o2 > 2400) return false;
            double charge = getNumber(cmd, "charge_limit_w");
            if (charge < 0 || charge > 20000) return false;
            double radiator = getNumber(cmd, "radiator_valve");
            if (radiator < 0 || radiator > 10000) return false;
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    private double getNumber(Map<String, Object> cmd, String key) {
        Object val = cmd.get(key);
        if (val instanceof Number) {
            return ((Number) val).doubleValue();
        }
        throw new IllegalArgumentException("missing " + key);
    }
}
