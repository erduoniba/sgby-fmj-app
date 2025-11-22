/**
 * 🔧 FMJ Game DevTools - External JavaScript Module
 * 
 * 这个文件包含游戏的所有开发和调试功能。
 * 只应该在开发模式下加载，绝对不能部署到生产环境。
 */

// 检查是否在开发环境
var isDevelopmentEnv = (function() {
    if (typeof window === 'undefined' || !window.location) {
        return true; // Node.js 环境或测试环境
    }
    
    var hostname = window.location.hostname;
    var protocol = window.location.protocol;
    
    return hostname === 'localhost' || 
           hostname === '127.0.0.1' || 
           protocol === 'file:';
})();

if (!isDevelopmentEnv) {
    console.warn('🛡️ DevTools 在非开发环境中被阻止加载');
} else {
    
    /**
     * FMJ Game DevTools 核心模块
     */
    window.FMJDevTools = {
        
        // 游戏实例引用
        gameInstance: null,
        
        /**
         * 初始化开发工具
         */
        initialize: function(gameInstance) {
            console.log('🔧 正在初始化 FMJ DevTools...');
            
            if (!gameInstance) {
                console.error('❌ 没有提供游戏实例给 DevTools');
                return false;
            }
            
            this.gameInstance = gameInstance;
            this.setupDevTools();
            console.log('✅ FMJ DevTools 初始化成功');
            return true;
        },
        
        /**
         * 设置开发工具函数
         */
        setupDevTools: function() {
            var self = this;
            
            // 暴露到 window.devTools 以保持向后兼容
            window.devTools = {
                gameInstance: self.gameInstance,
                cheatAddMoney: self.cheatAddMoney.bind(self),
                cheatFullHP: self.cheatFullHP.bind(self),
                cheatMaxStats: self.cheatMaxStats.bind(self),
                cheatAddExp: self.cheatAddExp.bind(self),
                addItem: self.addItem.bind(self),
                getGameInfo: self.getGameInfo.bind(self),
                openDebugMenu: self.openDebugMenu.bind(self)
            };
        },
        
        /**
         * 获取游戏实例
         */
        getGameInstance: function() {
            if (this.gameInstance) {
                return this.gameInstance;
            }
            
            return window.fmjGame || 
                   (window['fmj.core'] && window['fmj.core'].fmj && window['fmj.core'].fmj.game) ||
                   (window['fmj.core'] && window['fmj.core'].fmj && window['fmj.core'].fmj.MainGame && window['fmj.core'].fmj.MainGame.instance);
        },
        
        /**
         * 获取Player类引用
         */
        getPlayerClass: function() {
            return window['fmj.core'] && window['fmj.core'].fmj && window['fmj.core'].fmj.characters && window['fmj.core'].fmj.characters.Player;
        },
        
        /**
         * 作弊：增加金钱
         */
        cheatAddMoney: function(amount) {
            try {
                console.log('DEBUG: cheatAddMoney 调用，金额:', amount);
                var amountToAdd = amount || 99999;
                var PlayerClass = this.getPlayerClass();
                
                if (PlayerClass) {
                    if (PlayerClass.Companion && PlayerClass.Companion.sMoney !== undefined) {
                        var oldMoney = PlayerClass.Companion.sMoney;
                        PlayerClass.Companion.sMoney = (PlayerClass.Companion.sMoney || 0) + amountToAdd;
                        console.log('✅ 金钱从', oldMoney, '增加到', PlayerClass.Companion.sMoney);
                        return true;
                    } else if (PlayerClass.sMoney !== undefined) {
                        var oldMoney = PlayerClass.sMoney;
                        PlayerClass.sMoney = (PlayerClass.sMoney || 0) + amountToAdd;
                        console.log('✅ 金钱从', oldMoney, '增加到', PlayerClass.sMoney);
                        return true;
                    }
                }
                console.error('❌ 无法访问 Player.sMoney');
                return false;
            } catch(e) {
                console.error('❌ cheatAddMoney 失败:', e);
                return false;
            }
        },
        
        /**
         * 作弊：回复全HP/MP
         */
        cheatFullHP: function() {
            try {
                console.log('DEBUG: cheatFullHP 调用');
                var gameInstance = this.getGameInstance();
                
                if (gameInstance && gameInstance.playerList) {
                    for (var i = 0; i < gameInstance.playerList.size; i++) {
                        var player = gameInstance.playerList.get_za3lpa$(i);
                        if (player) {
                            player.hp = player.maxHP;
                            player.mp = player.maxMP;
                        }
                    }
                    console.log('✅ 所有玩家 HP/MP 已恢复');
                    return true;
                }
                return false;
            } catch(e) {
                console.error('❌ cheatFullHP 失败:', e);
                return false;
            }
        },
        
        /**
         * 作弊：最大化属性
         */
        cheatMaxStats: function() {
            try {
                console.log('DEBUG: cheatMaxStats 调用');
                var gameInstance = this.getGameInstance();
                
                if (gameInstance && gameInstance.playerList) {
                    for (var i = 0; i < gameInstance.playerList.size; i++) {
                        var player = gameInstance.playerList.get_za3lpa$(i);
                        if (player) {
                            player.attack = 999;
                            player.defend = 999;
                            player.speed = 999;
                            player.luck = 999;
                            player.lingli = 999;
                            player.maxHP = 9999;
                            player.maxMP = 9999;
                            player.hp = player.maxHP;
                            player.mp = player.maxMP;
                        }
                    }
                    console.log('✅ 所有玩家属性已最大化');
                    return true;
                }
                return false;
            } catch(e) {
                console.error('❌ cheatMaxStats 失败:', e);
                return false;
            }
        },
        
        /**
         * 作弊：增加经验值
         */
        cheatAddExp: function(amount) {
            try {
                console.log('DEBUG: cheatAddExp 调用，经验值:', amount);
                var expToAdd = amount || 10000;
                var gameInstance = this.getGameInstance();
                
                if (gameInstance && gameInstance.playerList && gameInstance.playerList.size > 0) {
                    for (var i = 0; i < gameInstance.playerList.size; i++) {
                        var player = gameInstance.playerList.get_za3lpa$(i);
                        if (player) {
                            var oldLevel = player.level || 1;
                            player.level = Math.min(oldLevel + 5, 99);
                            
                            var oldMaxHP = player.maxHP || 100;
                            var oldMaxMP = player.maxMP || 50;
                            player.maxHP = oldMaxHP + 50;
                            player.maxMP = oldMaxMP + 30;
                            player.hp = player.maxHP;
                            player.mp = player.maxMP;
                            
                            if (player.currentExp !== undefined) {
                                player.currentExp = (player.currentExp || 0) + expToAdd;
                            }
                            
                            console.log('✅ 玩家', i, '等级从', oldLevel, '升级到', player.level);
                        }
                    }
                    console.log('✅ 所有玩家处理完成');
                    return true;
                } else {
                    console.error('❌ 没有找到玩家或玩家列表无效');
                    return false;
                }
            } catch(e) {
                console.error('❌ cheatAddExp 失败:', e);
                return false;
            }
        },
        
        /**
         * 添加物品
         */
        addItem: function(type, index, count) {
            try {
                console.log('DEBUG: addItem 调用，类型:', type, '索引:', index, '数量:', count);
                var PlayerClass = this.getPlayerClass();
                var goodsList = null;
                
                // 调试信息：显示PlayerClass的结构
                console.log('DEBUG: PlayerClass:', PlayerClass);
                if (PlayerClass) {
                    console.log('DEBUG: PlayerClass.Companion:', PlayerClass.Companion);
                    if (PlayerClass.Companion) {
                        console.log('DEBUG: PlayerClass.Companion.sGoodsList:', PlayerClass.Companion.sGoodsList);
                    }
                    console.log('DEBUG: PlayerClass.sGoodsList:', PlayerClass.sGoodsList);
                }
                
                // 尝试多种方式获取 sGoodsList
                if (PlayerClass && PlayerClass.Companion && PlayerClass.Companion.sGoodsList) {
                    goodsList = PlayerClass.Companion.sGoodsList;
                    console.log('DEBUG: 通过 Companion 找到 sGoodsList');
                } else if (PlayerClass && PlayerClass.sGoodsList) {
                    goodsList = PlayerClass.sGoodsList;
                    console.log('DEBUG: 直接找到 sGoodsList');
                } else {
                    // 尝试通过不同的路径查找
                    var fmjCore = window['fmj.core'];
                    if (fmjCore && fmjCore.fmj && fmjCore.fmj.characters && fmjCore.fmj.characters.Player) {
                        var Player = fmjCore.fmj.characters.Player;
                        if (Player.Companion && Player.Companion.sGoodsList) {
                            goodsList = Player.Companion.sGoodsList;
                            console.log('DEBUG: 通过完整路径找到 sGoodsList');
                        }
                    }
                }
                
                // 尝试使用安全的 DevToolsIntegration.addItem 方法
                if (window.fmj && window.fmj.devtools && window.fmj.devtools.DevToolsIntegration) {
                    var devToolsIntegration = window.fmj.devtools.DevToolsIntegration;
                    if (typeof devToolsIntegration.addItem === 'function') {
                        var result = devToolsIntegration.addItem(type, index, count || 1);
                        if (result) {
                            // 成功时由 DevToolsIntegration 输出日志
                            return true;
                        } else {
                            // 失败时静默返回false（可能是无效物品）
                            return false;
                        }
                    }
                }
                
                // 回退到直接操作 goodsList（为了向后兼容）
                if (goodsList) {
                    // 尝试使用编译后的方法名
                    if (typeof goodsList.addGoods_qt1dr2$ === 'function') {
                        goodsList.addGoods_qt1dr2$(type, index, count || 1);
                        console.log('✅ 物品添加成功 (使用编译方法名)');
                        return true;
                    } else if (typeof goodsList.addGoods === 'function') {
                        goodsList.addGoods(type, index, count || 1);
                        console.log('✅ 物品添加成功');
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } catch(e) {
                // 对于批量操作，静默处理异常以避免大量错误日志
                // 只有在单独调用时才输出详细错误信息
                if (arguments.length > 3 && arguments[3] === true) {
                    // 第4个参数为true时输出详细日志（单独调用模式）
                    console.error('❌ addItem 失败:', e);
                    console.error('错误堆栈:', e.stack);
                }
                return false;
            }
        },
        
        /**
         * 获取游戏信息
         */
        getGameInfo: function() {
            try {
                var gameInstance = this.getGameInstance();
                var PlayerClass = this.getPlayerClass();
                
                var playerInfo = "无角色";
                var money = 0;
                var playerCount = 0;
                
                // 获取金钱
                if (PlayerClass) {
                    if (PlayerClass.Companion && PlayerClass.Companion.sMoney !== undefined) {
                        money = PlayerClass.Companion.sMoney;
                    } else if (PlayerClass.sMoney !== undefined) {
                        money = PlayerClass.sMoney;
                    }
                }
                
                // 获取玩家信息
                if (gameInstance && gameInstance.playerList && gameInstance.playerList.size > 0) {
                    playerCount = gameInstance.playerList.size;
                    try {
                        var player = gameInstance.playerList.get_za3lpa$(0);
                        if (player) {
                            playerInfo = "Level " + (player.level || 1) + " " + (player.name || "角色");
                        }
                    } catch(e) {
                        console.log('无法获取玩家信息:', e);
                    }
                }
                
                return {
                    player: playerInfo,
                    money: money,
                    map: "当前场景",
                    playerCount: playerCount
                };
            } catch(e) {
                console.error('❌ getGameInfo 失败:', e);
                return {
                    player: "获取失败",
                    money: 0,
                    map: "未知",
                    error: e.message
                };
            }
        },
        
        /**
         * 打开调试菜单
         */
        openDebugMenu: function() {
            try {
                console.log('DEBUG: openDebugMenu 调用');
                var gameInstance = this.getGameInstance();
                
                if (gameInstance && gameInstance.mainScene && gameInstance.mainScene.onKeyUp_za3lpa$) {
                    gameInstance.mainScene.onKeyUp_za3lpa$(10); // KEY_DEBUG = 10
                    console.log('✅ 调试菜单键已发送');
                    return true;
                } else {
                    console.error('❌ 无法访问 mainScene');
                    return false;
                }
            } catch(e) {
                console.error('❌ openDebugMenu 失败:', e);
                return false;
            }
        }
    };
    
    console.log('📦 FMJ DevTools 模块加载成功');
}