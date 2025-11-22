/**
 * 宝箱映射 localStorage 持久化管理器
 * 用于保存和加载游戏中宝箱与事件ID的映射关系
 */

(function(window) {
    'use strict';
    
    // localStorage 管理器
    const BoxMappingStorage = {
        /**
         * 获取当前游戏 MOD 名称
         * @returns {string} 游戏 MOD 名称
         */
        getGameMod: function() {
            return window.choiceLibName || 'FMJ';
        },
        
        /**
         * 保存宝箱映射到 localStorage
         * @param {Object} mapping - 宝箱映射对象
         */
        save: function(mapping) {
            try {
                const gameMod = this.getGameMod();
                const storageKey = `box_mapping_${gameMod}`;
                const jsonStr = JSON.stringify(mapping);
                
                localStorage.setItem(storageKey, jsonStr);
                
                console.log(`[BOX_MAPPING_STORAGE] Saved ${Object.keys(mapping).length} mappings for ${gameMod}`);
            } catch (e) {
                console.error('[BOX_MAPPING_STORAGE] Failed to save to localStorage:', e);
            }
        },
        
        /**
         * 从 localStorage 加载宝箱映射
         * @returns {Object} 宝箱映射对象，如果不存在则返回空对象
         */
        load: function() {
            try {
                const gameMod = this.getGameMod();
                const storageKey = `box_mapping_${gameMod}`;
                const jsonStr = localStorage.getItem(storageKey);
                
                if (jsonStr) {
                    const mapping = JSON.parse(jsonStr);
                    console.log(`[BOX_MAPPING_STORAGE] Loaded ${Object.keys(mapping).length} mappings for ${gameMod}`);
                    return mapping;
                } else {
                    console.log(`[BOX_MAPPING_STORAGE] No saved mappings found for ${gameMod}`);
                    return {};
                }
            } catch (e) {
                console.error('[BOX_MAPPING_STORAGE] Failed to load from localStorage:', e);
                return {};
            }
        },
        
        /**
         * 清除当前游戏的宝箱映射
         */
        clear: function() {
            try {
                const gameMod = this.getGameMod();
                const storageKey = `box_mapping_${gameMod}`;
                localStorage.removeItem(storageKey);
                console.log(`[BOX_MAPPING_STORAGE] Cleared storage for ${gameMod}`);
            } catch (e) {
                console.error('[BOX_MAPPING_STORAGE] Failed to clear storage:', e);
            }
        },
        
        /**
         * 获取所有游戏的宝箱映射键名列表
         * @returns {Array} 存储键名列表
         */
        list: function() {
            const keys = [];
            for (let i = 0; i < localStorage.length; i++) {
                const key = localStorage.key(i);
                if (key && key.startsWith('box_mapping_')) {
                    keys.push(key);
                }
            }
            return keys;
        },
        
        /**
         * 获取存储大小（字节）
         * @returns {number} 当前游戏映射的存储大小
         */
        getSize: function() {
            try {
                const gameMod = this.getGameMod();
                const storageKey = `box_mapping_${gameMod}`;
                const data = localStorage.getItem(storageKey);
                return data ? new Blob([data]).size : 0;
            } catch (e) {
                return 0;
            }
        }
    };
    
    // 与 Kotlin/JS 游戏引擎的交互桥接
    const BoxMappingBridge = {
        // 当前游戏的映射缓存
        currentMapping: {},
        
        /**
         * 初始化映射（自动识别游戏 MOD）
         */
        init: function() {
            this.currentMapping = BoxMappingStorage.load();
            const gameMod = BoxMappingStorage.getGameMod();
            console.log(`[BOX_MAPPING_BRIDGE] Initialized for ${gameMod} with ${Object.keys(this.currentMapping).length} mappings`);
        },
        
        /**
         * 添加宝箱映射
         * @param {string} boxKey - 宝箱唯一标识
         * @param {number} eventId - 事件ID
         */
        addMapping: function(boxKey, eventId) {
            if (!this.currentMapping[boxKey]) {
                this.currentMapping[boxKey] = eventId;
                console.log(`[BOX_MAPPING_BRIDGE] Added mapping: ${boxKey} -> Event ${eventId}`);
                
                // 自动保存到 localStorage
                BoxMappingStorage.save(this.currentMapping);
            }
        },
        
        /**
         * 获取宝箱对应的事件ID
         * @param {string} boxKey - 宝箱唯一标识
         * @returns {number|null} 事件ID，如果不存在则返回 null
         */
        getEventId: function(boxKey) {
            return this.currentMapping[boxKey] || null;
        },
        
        /**
         * 检查宝箱是否已收集
         * @param {string} boxKey - 宝箱唯一标识
         * @param {Array} globalEvents - 全局事件数组
         * @returns {boolean} 是否已收集
         */
        isBoxCollected: function(boxKey, globalEvents) {
            const eventId = this.getEventId(boxKey);
            return eventId !== null && globalEvents[eventId] === true;
        },
        
        /**
         * 清除当前游戏的映射
         */
        clear: function() {
            this.currentMapping = {};
            BoxMappingStorage.clear();
        },
        
        /**
         * 获取当前所有映射
         * @returns {Object} 当前映射对象
         */
        getAllMappings: function() {
            return { ...this.currentMapping };
        }
    };
    
    // 导出到全局对象
    window.BoxMappingStorage = BoxMappingStorage;
    window.BoxMappingBridge = BoxMappingBridge;
    
    // 供 Kotlin/JS 调用的全局函数
    window.boxMapping = {
        // 初始化映射（游戏启动或切换时调用）
        init: function() {
            BoxMappingBridge.init();
        },
        
        // 添加映射（发现新的宝箱-事件关系时调用）
        add: function(boxKey, eventId) {
            BoxMappingBridge.addMapping(boxKey, eventId);
        },
        
        // 获取事件ID
        getEventId: function(boxKey) {
            return BoxMappingBridge.getEventId(boxKey);
        },
        
        // 检查是否已收集
        isCollected: function(boxKey, globalEvents) {
            return BoxMappingBridge.isBoxCollected(boxKey, globalEvents);
        },
        
        // 获取所有映射
        getAll: function() {
            return BoxMappingBridge.getAllMappings();
        },
        
        // 清除映射
        clear: function() {
            BoxMappingBridge.clear();
        }
    };
    
    console.log('[BOX_MAPPING_STORAGE] Module loaded using localStorage');
    
})(window);