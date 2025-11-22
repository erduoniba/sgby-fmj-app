/**
 * 滤镜和游戏速度桥接文件
 * 提供统一的滤镜控制和游戏速度控制API，供iOS原生代码调用
 * 适用于三国霸业和伏魔记
 */

console.log("🎨 filter-bridge.js 开始加载...");

// 游戏速度控制对象
window.GameSpeedController = {
  // 当前游戏速度倍数
  currentSpeed: 1.0,
  
  // 支持的预设速度
  presetSpeeds: [0.5, 1.0, 1.5, 2.0, 3.0],
  
  /**
   * 设置游戏速度
   * @param {number} multiple - 速度倍数
   * @returns {boolean} - 是否设置成功
   */
  setGameSpeed: function(multiple) {
    console.log('⚡ 设置游戏速度:', multiple);
    
    if (typeof multiple !== 'number' || multiple <= 0) {
      console.error('❌ 游戏速度倍数必须是大于0的数字');
      return false;
    }
    
    // 限制速度范围，避免极端值影响游戏体验
    if (multiple > 5.0) {
      multiple = 5.0;
      console.warn('⚠️ 游戏速度被限制为最大值 5.0');
    } else if (multiple < 0.2) {
      multiple = 0.2;
      console.warn('⚠️ 游戏速度被限制为最小值 0.2');
    }
    
    this.currentSpeed = multiple;
    
    // 三国霸业的速度控制 - 通过WebAssembly模块
    if (typeof Module !== 'undefined' && Module._bayeSetSpeed && Module.calledRun) {
      Module._bayeSetSpeed(multiple);
      console.log('✅ 通过 Module._bayeSetSpeed 设置游戏速度:', multiple);
    } 
    // WebAssembly模块未初始化时延迟执行
    else if (typeof Module !== 'undefined' && Module._bayeSetSpeed && !Module.calledRun) {
      console.log('⏳ WebAssembly模块未初始化，延迟设置游戏速度');
      var pendingSpeed = multiple;
      var maxRetries = 50; // 最多重试50次（5秒）
      var retryCount = 0;
      var checkInitialized = function() {
        if (Module.calledRun) {
          Module._bayeSetSpeed(pendingSpeed);
          console.log('✅ 延迟设置游戏速度成功:', pendingSpeed);
        } else if (retryCount < maxRetries) {
          retryCount++;
          setTimeout(checkInitialized, 100);
        } else {
          console.warn('⚠️ 超时：WebAssembly模块初始化失败');
        }
      };
      setTimeout(checkInitialized, 100);
    }
    
    // 保存到localStorage
    try {
      localStorage.setItem('gameSpeed', multiple.toString());
    } catch (e) {
      console.warn('无法保存游戏速度设置到localStorage:', e);
    }
    
    console.log('✅ 游戏速度设置成功:', multiple + '倍');
    return true;
  },
  
  /**
   * 获取当前游戏速度
   * @returns {number} - 当前速度倍数
   */
  getCurrentSpeed: function() {
    return this.currentSpeed;
  },
  
  /**
   * 获取预设速度列表
   * @returns {Array} - 预设速度数组
   */
  getPresetSpeeds: function() {
    return this.presetSpeeds.slice(); // 返回副本
  },
  
  /**
   * 从localStorage恢复游戏速度设置
   */
  restoreSpeed: function() {
    try {
      const savedSpeed = localStorage.getItem('gameSpeed');
      if (savedSpeed) {
        const speed = parseFloat(savedSpeed);
        if (!isNaN(speed) && speed > 0) {
          this.setGameSpeed(speed);
          console.log('✅ 恢复游戏速度设置:', speed);
        }
      }
    } catch (e) {
      console.warn('无法从localStorage恢复游戏速度设置:', e);
    }
  },
  
  /**
   * 重置为默认速度
   */
  resetSpeed: function() {
    this.setGameSpeed(1.0);
  }
};

// 滤镜控制对象
window.FilterBridge = {
  // 当前滤镜状态
  currentFilter: 'none',
  
  // 支持的滤镜列表
  filters: {
    none: {
      name: '无滤镜',
      backgroundColor: 'transparent',
      opacity: '0',
      blendMode: 'normal'
    },
    vintage1980: {
      name: '复古1980',
      backgroundColor: 'rgba(255, 198, 145, 0.7)',
      opacity: '0.7',
      blendMode: 'multiply'
    },
    refreshing: {
      name: '清新明亮',
      backgroundColor: 'rgb(0, 255, 255)',
      opacity: '0.25',
      blendMode: 'overlay'
    },
    // 以下滤镜暂时禁用，保留供将来扩展
    /*
    green: {
      name: '绿色经典',
      backgroundColor: 'rgb(51, 204, 102)',
      opacity: '0.35',
      blendMode: 'color'
    },
    red: {
      name: '红色怀旧',
      backgroundColor: 'rgb(255, 51, 51)',
      opacity: '0.25',
      blendMode: 'color'
    },
    sepia: {
      name: '怀旧棕褐',
      backgroundColor: 'rgba(210, 180, 140, 0.5)',
      opacity: '0.5',
      blendMode: 'multiply'
    },
    night: {
      name: '夜间模式',
      backgroundColor: 'rgba(0, 0, 50, 0.3)',
      opacity: '0.3',
      blendMode: 'multiply'
    },
    warm: {
      name: '温暖',
      backgroundColor: 'rgba(255, 200, 100, 0.2)',
      opacity: '0.2',
      blendMode: 'overlay'
    },
    cool: {
      name: '冷色调',
      backgroundColor: 'rgba(100, 150, 255, 0.2)',
      opacity: '0.2',
      blendMode: 'overlay'
    }
    */
  },

  /**
   * 设置预设滤镜
   * @param {string} filterName - 滤镜名称
   * @returns {boolean} - 是否设置成功
   */
  setPresetFilter: function(filterName) {
    console.log('🎨 设置滤镜:', filterName);
    
    // 检查滤镜是否存在
    if (!this.filters[filterName]) {
      console.error('❌ 未知的滤镜名称:', filterName);
      return false;
    }

    // 获取或创建滤镜层
    let overlay = document.getElementById('filterOverlay');
    if (!overlay) {
      // 如果滤镜层不存在，创建它
      overlay = document.createElement('div');
      overlay.id = 'filterOverlay';
      overlay.className = 'filter-overlay';
      
      // 查找合适的父元素
      const app = document.getElementById('app');
      // const lcd = document.getElementById('lcd'); // 暂时注释掉未使用的变量
      const targetElement = app || document.body;
      
      // 如果有app元素，将滤镜层插入到app中
      if (app) {
        // 将滤镜层作为app的第二个子元素（canvas之后）
        const canvas = app.querySelector('canvas');
        if (canvas && canvas.nextSibling) {
          app.insertBefore(overlay, canvas.nextSibling);
        } else {
          app.appendChild(overlay);
        }
      } else {
        // 否则直接添加到body
        targetElement.appendChild(overlay);
      }
    }

    // 应用滤镜配置
    const filter = this.filters[filterName];
    overlay.style.backgroundColor = filter.backgroundColor;
    overlay.style.opacity = filter.opacity;
    overlay.style.mixBlendMode = filter.blendMode;
    
    // 确保滤镜层的基本样式
    overlay.style.position = 'absolute';
    overlay.style.top = '0';
    overlay.style.left = '0';
    overlay.style.width = '100%';
    overlay.style.height = '100%';
    overlay.style.pointerEvents = 'none';
    overlay.style.zIndex = '1000';
    overlay.style.transition = 'opacity 0.3s ease';

    // 更新当前滤镜状态
    this.currentFilter = filterName;
    
    // 保存到localStorage
    try {
      localStorage.setItem('gameFilter', filterName);
    } catch (e) {
      console.warn('无法保存滤镜设置到localStorage:', e);
    }

    console.log('✅ 滤镜设置成功:', filterName);
    return true;
  },

  /**
   * 获取当前滤镜
   * @returns {string} - 当前滤镜名称
   */
  getCurrentFilter: function() {
    return this.currentFilter;
  },

  /**
   * 获取所有可用滤镜列表
   * @returns {Array} - 滤镜信息数组
   */
  getAvailableFilters: function() {
    return Object.keys(this.filters).map(key => ({
      id: key,
      name: this.filters[key].name
    }));
  },

  /**
   * 从localStorage恢复滤镜设置
   */
  restoreFilter: function() {
    try {
      const savedFilter = localStorage.getItem('gameFilter');
      // 检查是否为有效的滤镜名称（非空字符串且在滤镜列表中）
      if (savedFilter && savedFilter.trim() !== '' && this.filters[savedFilter]) {
        this.setPresetFilter(savedFilter);
        console.log('✅ 恢复滤镜设置:', savedFilter);
      } else {
        // 清除无效的滤镜设置
        if (savedFilter) {
          localStorage.removeItem('gameFilter');
          console.log('🧹 清除无效的滤镜设置:', savedFilter);
        }
        console.log('⚪ 未找到有效的滤镜设置，使用默认设置');
      }
    } catch (e) {
      console.warn('无法从localStorage恢复滤镜设置:', e);
    }
  },

  /**
   * 清除滤镜
   */
  clearFilter: function() {
    this.setPresetFilter('none');
  },

  /**
   * 自定义滤镜
   * @param {Object} config - 滤镜配置
   */
  setCustomFilter: function(config) {
    let overlay = document.getElementById('filterOverlay');
    if (!overlay) {
      console.error('❌ 滤镜层不存在');
      return false;
    }

    if (config.backgroundColor) {
      overlay.style.backgroundColor = config.backgroundColor;
    }
    if (config.opacity !== undefined) {
      overlay.style.opacity = config.opacity;
    }
    if (config.blendMode) {
      overlay.style.mixBlendMode = config.blendMode;
    }

    this.currentFilter = 'custom';
    console.log('✅ 自定义滤镜设置成功');
    return true;
  }
};

// 为了兼容性，同时暴露全局函数
window.setPresetFilter = function(filterName) {
  return window.FilterBridge.setPresetFilter(filterName);
};

// 游戏速度全局API
window.setGameSpeed = function(multiple) {
  return window.GameSpeedController.setGameSpeed(multiple);
};

window.getGameSpeed = function() {
  return window.GameSpeedController.getCurrentSpeed();
};

// 游戏速度API别名
window.gameSpeedMultiple = window.setGameSpeed;

// 页面加载完成后恢复设置
if (document.readyState === 'loading') {
  document.addEventListener('DOMContentLoaded', function() {
    window.FilterBridge.restoreFilter();
    window.GameSpeedController.restoreSpeed();
  });
} else {
  // 如果文档已经加载完成
  window.FilterBridge.restoreFilter();
  window.GameSpeedController.restoreSpeed();
}

// 游戏引擎初始化后再次恢复速度设置
// 确保游戏速度设置生效
(function() {
  let retryCount = 0;
  const maxRetries = 20; // 增加重试次数
  let speedApplied = false;
  
  function ensureSpeedRestored() {
    // 检查是否有保存的速度设置
    const savedSpeed = localStorage.getItem('gameSpeed');
    if (!savedSpeed || speedApplied) return;
    
    const targetSpeed = parseFloat(savedSpeed);
    if (isNaN(targetSpeed) || targetSpeed <= 0) return;
    
    // 检查游戏引擎是否已初始化
    const engineReady = (typeof Module !== 'undefined' && Module.calledRun);
    
    if (engineReady) {
      // 强制应用速度设置，不检查当前速度
      console.log('🔧 强制应用游戏速度设置:', targetSpeed);
      window.GameSpeedController.setGameSpeed(targetSpeed);
      
      // 再次验证速度是否设置成功
      setTimeout(() => {
        if (typeof Module !== 'undefined' && Module._bayeSetSpeed) {
          Module._bayeSetSpeed(targetSpeed);
          console.log('✅ 直接调用Module._bayeSetSpeed:', targetSpeed);
        }
        speedApplied = true;
      }, 500);
    } else if (retryCount < maxRetries) {
      // 游戏引擎未就绪，稍后重试
      retryCount++;
      setTimeout(ensureSpeedRestored, 1000);
    }
  }
  
  // 多个时间点尝试恢复速度
  setTimeout(ensureSpeedRestored, 2000);
})();

console.log("✅ filter-bridge.js 加载完成");