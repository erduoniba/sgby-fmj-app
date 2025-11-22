/**
 * 性能监控和内存管理模块
 * 用于解决游戏运行半小时后卡顿的问题
 */

// 性能监控器
window.PerformanceMonitor = {
    startTime: Date.now(),
    frameCount: 0,
    lastFPSTime: Date.now(),
    fps: 0,
    memoryUsage: {},
    gcCount: 0,
    lastGCTime: 0,
    
    // 日志输出控制
    log: function(level, message) {
        if (this.config && this.config.isProduction) {
            // 生产环境只输出错误
            if (level === 'error') {
                console.error(message);
            }
        } else {
            // 开发环境输出所有日志
            switch(level) {
                case 'error': console.error(message); break;
                case 'warn': console.warn(message); break;
                case 'info': console.log(message); break;
                default: console.log(message);
            }
        }
    },
    
    // 配置参数（生产环境保守配置）
    config: {
        enableMonitoring: true,
        gcInterval: 600000, // 每10分钟执行一次垃圾回收
        memoryThreshold: 200 * 1024 * 1024, // 200MB内存阈值（更宽松）
        fpsWarningThreshold: 20, // FPS警告阈值
        enableAutoGC: true,
        enableCacheClean: false, // 默认禁用缓存清理，避免删除用户数据
        cacheCleanInterval: 1800000, // 每30分钟清理一次缓存（如果启用）
        isProduction: window.location.hostname !== 'localhost' && window.location.href.indexOf('debug=1') < 0,
        logLevel: 'error' // 生产环境只输出错误日志
    },
    
    // 初始化性能监控
    init: function() {
        if (!this.config.enableMonitoring) return;
        
        // 监控FPS
        this.startFPSMonitoring();
        
        // 监控内存使用
        this.startMemoryMonitoring();
        
        // 自动垃圾回收
        if (this.config.enableAutoGC) {
            this.startAutoGC();
        }
        
        // 定期清理缓存（如果启用）
        if (this.config.enableCacheClean) {
            this.startCacheCleanup();
        }
        
        this.log('info', '✅ 性能监控已启动');
    },
    
    // FPS监控
    startFPSMonitoring: function() {
        let lastTime = performance.now();
        let frames = 0;
        let lowFPSCount = 0; // 连续低FPS计数
        
        const measureFPS = () => {
            frames++;
            const currentTime = performance.now();
            
            if (currentTime >= lastTime + 1000) {
                this.fps = Math.round((frames * 1000) / (currentTime - lastTime));
                frames = 0;
                lastTime = currentTime;
                
                // FPS过低警告（需要连续3次低于阈值才触发）
                if (this.fps < this.config.fpsWarningThreshold) {
                    lowFPSCount++;
                    if (lowFPSCount >= 3) {
                        this.log('warn', `⚠️ FPS持续过低: ${this.fps}`);
                        // 生产环境不自动优化，避免影响用户体验
                        if (!this.config.isProduction) {
                            this.optimizePerformance();
                            lowFPSCount = 0; // 重置计数
                        }
                    }
                } else {
                    lowFPSCount = 0; // FPS恢复正常，重置计数
                }
            }
            
            requestAnimationFrame(measureFPS);
        };
        
        requestAnimationFrame(measureFPS);
    },
    
    // 内存监控
    startMemoryMonitoring: function() {
        if (!performance.memory) {
            console.warn('浏览器不支持内存监控');
            return;
        }
        
        setInterval(() => {
            this.memoryUsage = {
                used: performance.memory.usedJSHeapSize,
                total: performance.memory.totalJSHeapSize,
                limit: performance.memory.jsHeapSizeLimit,
                percentage: (performance.memory.usedJSHeapSize / performance.memory.jsHeapSizeLimit) * 100
            };
            
            // 内存使用过高警告
            if (this.memoryUsage.used > this.config.memoryThreshold) {
                this.log('warn', `⚠️ 内存使用过高: ${(this.memoryUsage.used / 1024 / 1024).toFixed(2)}MB`);
                this.triggerGC();
            }
        }, 5000);
    },
    
    // 自动垃圾回收
    startAutoGC: function() {
        setInterval(() => {
            const now = Date.now();
            if (now - this.lastGCTime > this.config.gcInterval) {
                this.triggerGC();
                this.lastGCTime = now;
            }
        }, 10000);
    },
    
    // 触发垃圾回收
    triggerGC: function() {
        this.log('info', '🔧 执行垃圾回收...');
        
        // 清理WebAssembly内存（如果游戏引擎支持）
        // 三国霸业的垃圾回收
        if (typeof Module !== 'undefined' && Module._bayeGCCheckAll) {
            try {
                Module._bayeGCCheckAll();
                this.log('info', '✅ 执行三国霸业垃圾回收');
            } catch (e) {
                this.log('error', '三国霸业GC失败: ' + e);
            }
        }
        
        // 清理Canvas缓存
        this.clearCanvasCache();
        
        // 清理事件监听器
        this.cleanupEventListeners();
        
        // 清理定时器
        this.cleanupTimers();
        
        // 清理DOM缓存
        this.cleanupDOMCache();
        
        this.gcCount++;
        this.log('info', `✅ 垃圾回收完成 (第${this.gcCount}次)`);
    },
    
    // 清理Canvas缓存
    clearCanvasCache: function() {
        // 注意：不要清空Canvas内容，这会导致画面白屏
        // 仅重置渲染上下文的一些属性以释放内存
        const canvas = document.getElementById('lcd');
        if (canvas && canvas.getContext) {
            const ctx = canvas.getContext('2d');
            
            if (ctx) {
                // 重置变换矩阵
                ctx.setTransform(1, 0, 0, 1, 0, 0);
                
                // 重置图像平滑设置
                ctx.imageSmoothingEnabled = false;
                ctx.imageSmoothingQuality = 'low';
                
                // 清理路径缓存
                ctx.beginPath();
                
                // 不要调用 clearRect，这会清空画面导致白屏
                // 游戏引擎会自动管理画布内容的绘制
            }
        }
    },
    
    // 清理事件监听器
    cleanupEventListeners: function() {
        // 仅在开发环境中检查事件监听器
        // getEventListeners 只在 Chrome DevTools 中可用
        if (typeof getEventListeners === 'function') {
            const events = ['touchstart', 'touchmove', 'touchend', 'click', 'keydown'];
            events.forEach(eventName => {
                try {
                    const listeners = getEventListeners(document)[eventName];
                    if (listeners && listeners.length > 10) {
                        console.warn(`事件 ${eventName} 有 ${listeners.length} 个监听器，可能存在泄漏`);
                    }
                } catch (e) {
                    // 忽略错误
                }
            });
        }
    },
    
    // 清理定时器
    cleanupTimers: function() {
        // 记录活动的定时器数量
        if (window.activeTimers) {
            const timerCount = Object.keys(window.activeTimers).length;
            if (timerCount > 50) {
                console.warn(`活动定时器过多: ${timerCount}`);
            }
        }
    },
    
    // 清理DOM缓存
    cleanupDOMCache: function() {
        // 清理jQuery缓存（如果使用jQuery）
        if (typeof $ !== 'undefined' && $.cache) {
            const cacheSize = Object.keys($.cache).length;
            if (cacheSize > 100) {
                console.warn(`jQuery缓存过大: ${cacheSize}`);
                // 清理未使用的缓存
                for (let key in $.cache) {
                    if (!document.getElementById(key)) {
                        delete $.cache[key];
                    }
                }
            }
        }
    },
    
    // 缓存清理
    startCacheCleanup: function() {
        setInterval(() => {
            this.cleanupCache();
        }, this.config.cacheCleanInterval);
    },
    
    cleanupCache: function() {
        this.log('info', '🧹 清理缓存...');
        
        // 注意：localStorage清理已禁用，避免删除用户重要数据
        // 如需启用，请设置 config.enableCacheClean = true
        // 并确保用户了解可能的数据丢失风险
        
        if (!this.config.enableCacheClean) {
            return;
        }
        
        // 仅清理明确标记为临时的数据
        const tempKeys = ['baye/temp', 'baye/cache'];
        const allKeys = Object.keys(localStorage);
        
        allKeys.forEach(key => {
            // 只清理临时数据，不删除存档
            if (tempKeys.some(prefix => key.startsWith(prefix))) {
                try {
                    localStorage.removeItem(key);
                    this.log('info', `清理临时数据: ${key}`);
                } catch (e) {
                    this.log('error', '清理缓存失败: ' + e);
                }
            }
        });
        
        // 清理图像缓存
        if (window.imageCache && Object.keys(window.imageCache).length > 100) {
            const keys = Object.keys(window.imageCache);
            const toDelete = keys.slice(0, keys.length - 50);
            toDelete.forEach(key => delete window.imageCache[key]);
            this.log('info', `清理图像缓存: ${toDelete.length} 项`);
        }
    },
    
    // 性能优化（生产环境更保守）
    optimizePerformance: function() {
        this.log('info', '⚡ 执行性能优化...');
        
        // 降低渲染质量
        const canvas = document.getElementById('lcd');
        if (canvas && canvas.getContext) {
            const ctx = canvas.getContext('2d');
            if (ctx) {
                ctx.imageSmoothingEnabled = false;
                ctx.imageSmoothingQuality = 'low';
            }
        }
        
        // 触发垃圾回收
        this.triggerGC();
    },
    
    // 获取性能报告
    getReport: function() {
        const runtime = (Date.now() - this.startTime) / 1000 / 60; // 分钟
        
        return {
            runtime: runtime.toFixed(2) + ' 分钟',
            fps: this.fps,
            memory: this.memoryUsage,
            gcCount: this.gcCount,
            status: this.fps > 30 ? '良好' : (this.fps > 20 ? '一般' : '较差')
        };
    },
    
    // 显示性能统计
    showStats: function() {
        const report = this.getReport();
        console.log('📊 性能报告:');
        console.log(`运行时间: ${report.runtime}`);
        console.log(`FPS: ${report.fps}`);
        console.log(`内存使用: ${(report.memory.used / 1024 / 1024).toFixed(2)}MB / ${(report.memory.limit / 1024 / 1024).toFixed(2)}MB`);
        console.log(`垃圾回收次数: ${report.gcCount}`);
        console.log(`性能状态: ${report.status}`);
    }
};

// 定时器跟踪（仅在开发环境启用）
// 生产环境不应重写全局函数，避免影响其他功能
if (window.location.hostname === 'localhost' || window.location.href.indexOf('debug=1') >= 0) {
    window.activeTimers = {};
    console.log('⚠️ 调试模式：定时器跟踪已启用');
}

// Canvas优化已移除
// 原因：创建了未使用的离屏Canvas和永久定时器，造成内存泄漏
// 游戏引擎已有自己的渲染优化机制

// 监听页面可见性变化，暂停/恢复游戏
document.addEventListener('visibilitychange', function() {
    if (document.hidden) {
        // 页面隐藏时暂停游戏（如果游戏支持）
        if (window.pauseGame) window.pauseGame();
    } else {
        // 页面恢复时恢复游戏（如果游戏支持）
        if (window.resumeGame) window.resumeGame();
        // 不自动执行垃圾回收，避免切换页面时卡顿
    }
});

// 导出全局对象
window.PerformanceMonitor = PerformanceMonitor;

// 自动初始化（延迟执行，等待游戏加载完成）
// 延迟更长时间，确保游戏速度设置已恢复
setTimeout(() => {
    // 仅在游戏实际运行时才启动监控
    if (typeof Module !== 'undefined') {
        PerformanceMonitor.init();
    } else {
        PerformanceMonitor.log('info', '⏳ 游戏未加载，延迟启动性能监控');
        setTimeout(() => {
            PerformanceMonitor.init();
        }, 5000);
    }
}, 10000); // 延迟到10秒，确保游戏引擎完全初始化