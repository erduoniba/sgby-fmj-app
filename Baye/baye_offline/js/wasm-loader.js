/**
 * WASM 加载器
 * 负责预加载 WebAssembly 模块并动态加载游戏引擎脚本
 * 支持 iOS 14+ 和 Android 平台
 */

(function() {
    'use strict';

    // iOS 14 WebAssembly 兼容性补丁
    if (!WebAssembly.instantiateStreaming) {
        console.log('添加 WebAssembly.instantiateStreaming polyfill');
        WebAssembly.instantiateStreaming = async function(response, imports) {
            const source = await (await response).arrayBuffer();
            return await WebAssembly.instantiate(source, imports);
        };
    }

    // iOS 14 WASM预加载 - 增加内存配置
    window.Module = window.Module || {
        // 设置足够的内存
        INITIAL_MEMORY: 67108864, // 64MB
        // 禁用内存增长
        ALLOW_MEMORY_GROWTH: false,
        // iOS 14 特别兼容设置
        onRuntimeInitialized: function() {
            console.log('✅ WebAssembly 运行时初始化完成 (iOS 14兼容模式)');
        }
    };

    /**
     * 加载 WASM 文件和游戏脚本
     */
    async function loadWasmAndScript() {
        console.log('开始预加载 WASM 文件 (Android兼容模式)...');
        try {
            // 使用 XMLHttpRequest 替代 fetch，支持 file:// 协议
            const wasmArrayBuffer = await new Promise((resolve, reject) => {
                const xhr = new XMLHttpRequest();
                xhr.open('GET', 'js/baye.v2.wasm', true);
                xhr.responseType = 'arraybuffer';

                xhr.onload = function() {
                    if (xhr.status === 200 || xhr.status === 0) { // status 0 for file://
                        resolve(xhr.response);
                    } else {
                        reject(new Error('WASM加载失败，状态码: ' + xhr.status));
                    }
                };

                xhr.onerror = function() {
                    reject(new Error('WASM加载网络错误'));
                };

                xhr.send();
            });

            // 验证WASM文件完整性
            if (wasmArrayBuffer.byteLength < 1000) {
                throw new Error('WASM文件大小异常: ' + wasmArrayBuffer.byteLength);
            }

            // 验证WASM魔术字节
            const view = new Uint8Array(wasmArrayBuffer, 0, 4);
            if (view[0] !== 0x00 || view[1] !== 0x61 || view[2] !== 0x73 || view[3] !== 0x6d) {
                throw new Error('WASM魔术字节无效');
            }

            window.Module.wasmBinary = wasmArrayBuffer;
            console.log('✅ WASM 文件预加载成功，大小:', wasmArrayBuffer.byteLength);

            // WASM预加载完成后，动态加载baye.v2.js
            const script = document.createElement('script');
            script.src = 'js/baye.v2.js?ver=202508130929';
            script.onload = function() {
                console.log('✅ baye.v2.js 加载完成');
            };
            script.onerror = function() {
                console.error('❌ baye.v2.js 加载失败');
            };
            document.head.appendChild(script);

        } catch (e) {
            console.error('❌ WASM 文件预加载失败:', e);
            // 失败时尝试同步加载
            const script = document.createElement('script');
            script.src = 'js/baye.v2.js?ver=202508130929';
            script.onload = function() {
                console.log('⚠️ 降级到同步加载模式');
            };
            document.head.appendChild(script);
        }
    }

    // 导出到全局
    window.loadWasmAndScript = loadWasmAndScript;

    // 页面加载完成后开始加载
    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', loadWasmAndScript);
    } else {
        loadWasmAndScript();
    }
})();
