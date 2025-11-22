//
//  HDBayeHooks.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/4/11.
//

import Foundation

class HDBayeHooks: NSObject {
    static let bayehooksJS = """
        if (typeof baye === 'undefined') {
            window.baye = {};
        }
        if (typeof baye.hooks === 'undefined') {
            baye.hooks = {};
        }
        baye.hooks.calcAttackRange = function(context) {
            // 李世民id为109
            if (context.personIndex == 109 && baye.data.g_Persons.length > 109) {
                console.log('calcAttackRange：' + context.type);
                var person = baye.data.g_Persons[109];
                if (person) {
                    console.log(`李世民受到攻击，快快增援`) 
                    person.Arms = person.Arms + 3000 
                    person.Force = person.Force + 20
                    person.IQ = person.IQ + 20
                }
            }
        }
    """
    
    static func bayeResolutionJS(_ resolution: String) -> String {
        // 调用离线包中统一的分辨率设置API
        let jsCode = """
            if (typeof window.setBayeResolution === 'function') {
                window.setBayeResolution('\(resolution)');
            } else {
                console.warn('setBayeResolution API 未找到，可能页面还未完全加载');
                // 保存设置，待页面加载完成后应用
                localStorage.setItem('baye/resolution', '\(resolution)');
            }
            """
        return jsCode
    }
}
