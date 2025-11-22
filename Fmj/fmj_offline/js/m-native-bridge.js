/**
 * 原生交互桥接文件
 * 包含与iOS/Android原生代码交互的所有JavaScript函数
 */

console.log("📱 m-native-bridge.js 开始加载...");

// iPhone检测函数 - 全局函数
window.isIPhone = function () {
  return navigator.userAgent.match(/(iPhone|iPod)/i);
};

// 初始化配置 - 从localStorage读取原生设置
window.choiceLibName = localStorage.getItem("choiceLibName") || "FMJ";
window.fmjcorev2 =
  localStorage.getItem("fmjcorev2") === "true" ||
  localStorage.getItem("fmjcorev2") === null;

// 从localStorage读取地图容器显示状态
window.mapContainerState = localStorage.getItem("mapContainerState") === "true";

// 地图图片状态
window.hasMapImage = false;

// 检查是否应该显示地图容器（三个必须条件）
function shouldShowMapContainer() {
  // 检查设备是否处于竖屏状态
  const isPortrait = window.innerHeight > window.innerWidth;
  
  return (
    window.mapContainerState === true &&
    window.hasMapImage === true &&
    isPortrait
  );
}

// 更新地图容器显示状态
function updateMapContainerDisplay() {
  const mapContainer = document.getElementById("map-container");
  if (mapContainer) {
    const shouldShow = shouldShowMapContainer();
    mapContainer.style.display = shouldShow ? "block" : "none";

    if (shouldShow) {
      // 获取地图预览图片元素来检查图片尺寸
      const mapPreview = document.getElementById("map-preview");
      const appElement = document.getElementById("app");
      
      // 计算可用空间
      const availableWidth = window.innerWidth - 10; // 减去边距
      const maxContainerWidth = Math.min(availableWidth, 600);
      
      // 计算可用高度（考虑app元素位置和安全距离）
      let availableHeight = window.innerHeight;
      if (appElement) {
        const appRect = appElement.getBoundingClientRect();
        availableHeight = window.innerHeight - appRect.bottom + 60; // 减去app底部位置和安全距离
      } else {
        availableHeight = window.innerHeight - 160; // 预留更多空间
      }
      
      // 确保合理的高度范围：最小150px，最大不超过屏幕高度的60%
      const maxContainerHeight = Math.min(
        Math.max(availableHeight, 150),
        window.innerHeight * 0.8
      );
      
      // 如果图片已加载，根据图片尺寸调整容器大小
      if (mapPreview && mapPreview.naturalWidth > 0 && mapPreview.naturalHeight > 0) {
        const imageAspectRatio = mapPreview.naturalWidth / mapPreview.naturalHeight;
        
        // 计算按宽度约束时的尺寸
        const widthConstrainedWidth = maxContainerWidth;
        const widthConstrainedHeight = widthConstrainedWidth * (mapPreview.naturalHeight / mapPreview.naturalWidth);
        
        // 计算按高度约束时的尺寸
        const heightConstrainedHeight = maxContainerHeight;
        const heightConstrainedWidth = heightConstrainedHeight * imageAspectRatio;
        
        // 选择不超出任何约束的尺寸
        let finalWidth, finalHeight;
        if (widthConstrainedHeight <= maxContainerHeight) {
          // 宽度是约束因子
          finalWidth = widthConstrainedWidth;
          finalHeight = widthConstrainedHeight;
        } else {
          // 高度是约束因子
          finalWidth = heightConstrainedWidth;
          finalHeight = heightConstrainedHeight;
        }
        
        mapContainer.style.width = finalWidth + "px";
        mapContainer.style.maxHeight = finalHeight + "px";
        
        console.log("Map container sized based on image:", {
          imageSize: [mapPreview.naturalWidth, mapPreview.naturalHeight],
          aspectRatio: imageAspectRatio.toFixed(3),
          screenSize: [window.innerWidth, window.innerHeight],
          appBottom: appElement ? appElement.getBoundingClientRect().bottom : 'N/A',
          availableHeight: availableHeight,
          constraints: [maxContainerWidth, maxContainerHeight],
          widthConstrained: [widthConstrainedWidth, Math.round(widthConstrainedHeight)],
          heightConstrained: [Math.round(heightConstrainedWidth), heightConstrainedHeight],
          finalSize: [Math.round(finalWidth), Math.round(finalHeight)],
          constraintType: widthConstrainedHeight <= maxContainerHeight ? 'width' : 'height'
        });
      } else {
        // 图片未加载时使用默认宽度
        mapContainer.style.width = maxContainerWidth + "px";
        mapContainer.style.maxHeight = maxContainerHeight + "px";
      }

      // 设置位置
      if (appElement) {
        const appRect = appElement.getBoundingClientRect();
        const appBottom = appRect.bottom;

        mapContainer.style.bottom = "auto";
        mapContainer.style.top = appBottom + 10 + "px";
      } else {
        mapContainer.style.bottom = "10px";
        mapContainer.style.top = "auto";
      }
    }
  }
}

// 重新评估地图显示状态（当任何条件改变时调用）
window.reevaluateMapDisplay = reevaluateMapDisplay;
function reevaluateMapDisplay() {
  updateMapContainerDisplay();
}

// 页面加载完成后初始化地图容器状态
window.addEventListener("DOMContentLoaded", function () {
  updateMapContainerDisplay();
});

// 游戏金币、经验和物品掉落，默认是1倍速
window.winMoneyMultiple = 1;
window.winExpMultiple = 1;
window.winItemMultiple = 1;

// 魔法列表是否反转，默认是true
window.magicReverse = true;

window.libsList = libsList;
function libsList() {
  return {
    FMJ: "伏魔记",
    FMJWMB: "伏魔记完美版（旭哥出品）",
    JYQXZ: "金庸群侠传",
    XKX: "侠客行",
    CBZZZSYZF: "赤壁之战之谁与争峰",
    YZCQ2: "一中传奇2",
    XXJWMB: "仙剑奇侠传完美版",
    XJQXZEZHJFJ: "仙剑奇侠传二之虎啸飞剑",
    XJQXZSZLHQY: "仙剑奇侠传三之轮回情缘",
    XJQXZSHYMYX: "仙剑奇侠传四之回梦游仙",
    LGSCQ: '伏魔记之老观寺传奇（旭哥出品）',
    XBMT: '新版魔塔',
    FMJLL: '伏魔记乐乐圆梦(木子出品)',
    XBMT: '新版魔塔',
    YXTS: '英雄坛说',
    WDSJ: '我的世界',
    FMJYMQZQ: '伏魔记之圆梦前奏曲(木子01)',
    FMJSNLWQ: '伏魔记之神女轮舞曲(木子02)',
    FMJMVKXQ: '伏魔记之魔女狂想曲(木子03)',
    FMJHMAHQ: '伏魔记之回梦安魂曲(木子04)',
    FMJFYJ: '伏魔记之伏羊记'
  };
}

window.joystickModeChanged = joystickModeChanged;
function joystickModeChanged(mode) {
  if (mode == 0) {
    // 摇杆模式
    $("#zone").show();
    $("#direction_pad").hide();

    // 确保摇杆已初始化
    if (!joystick) {
      initJoystick();
    }
  } else if (mode == 1) {
    // 按钮模式
    $("#zone").hide();
    if (joystick) {
      joystick.destroy();
      joystick = null;
    }

    // 显示方向按钮，位置由CSS媒体查询控制
    var dirPad = $("#direction_pad");
    dirPad.show();

    // 清除可能存在的内联样式，让CSS媒体查询控制位置
    dirPad.attr('style', 'display: block;');
  }
}


// 滤镜控制对象 - 提供统一的滤镜API
window.FilterBridge = {
  currentFilter: 'none',
  
  filters: {
    none: {
      backgroundColor: 'transparent',
      opacity: '0',
      blendMode: 'normal'
    },
    vintage1980: {
      backgroundColor: 'rgba(255, 198, 145, 0.7)',
      opacity: '0.7',
      blendMode: 'multiply'
    },
    refreshing: {
      backgroundColor: 'rgb(0, 255, 255)',
      opacity: '0.25',
      blendMode: 'overlay'
    }
    // 红色和绿色滤镜已移除
  },

  setPresetFilter: function(presetName) {
    console.log('🎨 设置滤镜:', presetName);

    const overlay = document.getElementById("filterOverlay");
    const mapOverlay = document.getElementById("mapFilterOverlay");

    if (!overlay) {
      console.error("滤镜层元素未找到");
      return false;
    }

    // 同步overlay的位置和大小
    overlay.style.position = "absolute";
    overlay.style.top = "0";
    overlay.style.left = "0";
    overlay.style.width = "100%";
    overlay.style.height = "100%";

    const filter = this.filters[presetName] || this.filters.none;

    // 应用滤镜效果到app元素的滤镜层
    overlay.style.backgroundColor = filter.backgroundColor;
    overlay.style.opacity = filter.opacity;
    overlay.style.mixBlendMode = filter.blendMode;

    // 同时应用滤镜效果到地图滤镜层
    if (mapOverlay) {
      mapOverlay.style.backgroundColor = filter.backgroundColor;
      mapOverlay.style.opacity = filter.opacity;
      mapOverlay.style.mixBlendMode = filter.blendMode;
    }

    this.currentFilter = presetName;

    // 保存到localStorage
    try {
      localStorage.setItem('gameFilter', presetName);
    } catch (e) {
      console.warn('无法保存滤镜设置:', e);
    }

    return true;
  },

  getCurrentFilter: function() {
    return this.currentFilter;
  },

  restoreFilter: function() {
    try {
      const savedFilter = localStorage.getItem('gameFilter');
      if (savedFilter && this.filters[savedFilter]) {
        this.setPresetFilter(savedFilter);
      }
    } catch (e) {
      console.warn('无法恢复滤镜设置:', e);
    }
  }
};

// 为了兼容性，保留全局函数
window.setPresetFilter = function(presetName) {
  return window.FilterBridge.setPresetFilter(presetName);
};

// 页面加载完成后恢复滤镜设置
window.addEventListener('DOMContentLoaded', function() {
  window.FilterBridge.restoreFilter();
});

// 游戏速度，默认是1倍速
function setGameSpeed(multiple) {
  window.gameSpeedMultiple = multiple;
  return true;
}

// 显示/隐藏地图容器
window.showMapContainer = showMapContainer;
function showMapContainer(show) {
  // 保存状态到localStorage
  window.mapContainerState = show;
  localStorage.setItem("mapContainerState", show.toString());

  // 更新地图容器显示
  updateMapContainerDisplay();
}

// 更新玩家位置的闪烁点
function updatePlayerPosition(
  relativeX,
  relativeY,
  mapX,
  mapY,
  mapWidth,
  mapHeight
) {
  // 保存当前位置数据
  window.currentPlayerPosition = {
    relativeX,
    relativeY,
    mapX,
    mapY,
    mapWidth,
    mapHeight,
  };

  const mapPreview = document.getElementById("map-preview");
  const playerDot = document.getElementById("player-dot");
  const playerCoords = document.getElementById("player-coords");
  const mapContainer = document.getElementById("map-container");

  if (!mapPreview || !playerDot || !mapContainer) return;

  // 等待图片加载完成
  if (mapPreview.naturalWidth === 0) {
    mapPreview.onload = function () {
      positionPlayerDot();
    };
    return;
  }

  positionPlayerDot();

  function positionPlayerDot() {
    // 获取图片的实际显示尺寸
    const imgRect = mapPreview.getBoundingClientRect();
    const containerRect = mapContainer.getBoundingClientRect();

    // 计算一个tile在当前显示尺寸下的大小
    // 每个tile在原始地图中是16x16像素
    const tileDisplayWidth = imgRect.width / mapWidth;
    const tileDisplayHeight = imgRect.height / mapHeight;

    // 点的大小应该是一个tile的大小，但不小于4px，不大于20px
    const dotSize = Math.max(
      4,
      Math.min(20, Math.min(tileDisplayWidth, tileDisplayHeight))
    );

    // 计算玩家在图片上的像素位置
    const dotX = relativeX * imgRect.width + tileDisplayWidth / 2;
    const dotY = relativeY * imgRect.height + tileDisplayHeight / 2;

    // 相对于容器的位置
    const imgOffsetX = mapPreview.offsetLeft;
    const imgOffsetY = mapPreview.offsetTop;

    // 更新点的大小和位置
    const halfDotSize = dotSize / 2;
    playerDot.style.width = dotSize + "px";
    playerDot.style.height = dotSize + "px";
    playerDot.style.left = imgOffsetX + dotX - halfDotSize + "px";
    playerDot.style.top = imgOffsetY + dotY - halfDotSize + "px";
    playerDot.style.display = "block";
    
    // 更新坐标显示
    if (playerCoords) {
      // playerCoords.textContent = `(${mapX},${mapY})`;
      // // 将坐标文本放在红点旁边，稍微偏右上方
      // playerCoords.style.left = imgOffsetX + dotX - dotSize - 2 + "px";
      // playerCoords.style.top = imgOffsetY + dotY - halfDotSize - 10 + "px";
      // playerCoords.style.display = "block";
    }

    console.log("Player position updated:", {
      mapPos: [mapX, mapY],
      mapSize: [mapWidth, mapHeight],
      relative: [relativeX, relativeY],
      pixel: [dotX, dotY],
      imgSize: [imgRect.width, imgRect.height],
      tileSize: [tileDisplayWidth, tileDisplayHeight],
      dotSize: dotSize,
      offset: [imgOffsetX, imgOffsetY],
    });
  }
}

// 添加激活状态闪动动画样式（只添加一次）
function addActiveBlinkingStyles() {
  if (document.getElementById('active-blinking-styles')) return;
  
  const style = document.createElement('style');
  style.id = 'active-blinking-styles';
  style.textContent = `
    @keyframes blinkActive {
      0% {
        background-color: #ffeb3b;
        box-shadow: 0 0 8px rgba(255,235,59,1);
      }
      25% {
        background-color: #ffc107;
        box-shadow: 0 0 12px rgba(255,193,7,1);
      }
      50% {
        background-color: #ff9800;
        box-shadow: 0 0 16px rgba(255,152,0,1);
      }
      75% {
        background-color: #ffc107;
        box-shadow: 0 0 12px rgba(255,193,7,1);
      }
      100% {
        background-color: #ffeb3b;
        box-shadow: 0 0 8px rgba(255,235,59,1);
      }
    }
    
    .active-blinking {
      animation: blinkActive 1s infinite;
    }
  `;
  document.head.appendChild(style);
}

// 更新宝箱位置信息
function updateTreasureBoxes(treasureBoxes, mapWidth, mapHeight) {
  // 确保动画样式已添加
  addActiveBlinkingStyles();
  
  // 保存当前宝箱数据
  currentTreasureBoxes = { treasureBoxes, mapWidth, mapHeight };

  const mapContainer = document.getElementById("map-container");
  const mapPreview = document.getElementById("map-preview");

  if (!mapContainer || !mapPreview) return;

  // 移除现有的所有宝箱点
  const existingBoxes = mapContainer.querySelectorAll(".treasure-box");
  existingBoxes.forEach((box) => box.remove());

  // 等待图片加载完成
  if (mapPreview.naturalWidth === 0) {
    mapPreview.onload = function () {
      positionTreasureBoxes();
    };
    return;
  }

  positionTreasureBoxes();

  function positionTreasureBoxes() {
    // 获取图片的实际显示尺寸
    const imgRect = mapPreview.getBoundingClientRect();
    const containerRect = mapContainer.getBoundingClientRect();

    // 计算一个tile在当前显示尺寸下的大小
    const tileDisplayWidth = imgRect.width / mapWidth;
    const tileDisplayHeight = imgRect.height / mapHeight;

    // 为每个宝箱创建标记点
    treasureBoxes.forEach((box, index) => {
      const boxElement = document.createElement("div");
      boxElement.className = "treasure-box";
      boxElement.dataset.boxIndex = index;
      boxElement.dataset.boxName = box.name;
      boxElement.dataset.isCollected = box.isCollected;

      // 计算宝箱在地图中的相对位置
      const relativeX = box.x / mapWidth;
      const relativeY = box.y / mapHeight;

      // 计算宝箱在图片上的像素位置
      const boxX = relativeX * imgRect.width + tileDisplayWidth;
      const boxY = relativeY * imgRect.height + tileDisplayWidth;

      // 相对于容器的位置
      const imgOffsetX = mapPreview.offsetLeft;
      const imgOffsetY = mapPreview.offsetTop;

      // 宝箱点的大小应该略小于tile大小，但不小于3px，不大于8px（缩小到一半）
      const boxSize = Math.max(
        3,
        Math.min(8, Math.min(tileDisplayWidth, tileDisplayHeight) * 0.4)
      );
      const halfBoxSize = boxSize / 2;

      // 设置宝箱样式和位置
      boxElement.style.width = boxSize + "px";
      boxElement.style.height = boxSize + "px";
      boxElement.style.left = imgOffsetX + boxX - boxSize / 2 + "px";
      boxElement.style.top = imgOffsetY + boxY + boxSize / 2 + "px";
      boxElement.style.position = "absolute";
      boxElement.style.borderRadius = "2px";
      boxElement.style.border = "1px solid #000";
      boxElement.style.zIndex = "20";
      boxElement.style.cursor = "pointer";

      // 根据宝箱状态设置颜色和效果
      if (box.isActive) {
        // 激活状态 - 添加闪动效果（如伏魔灯）
        boxElement.style.backgroundColor = "#ff0"; // 黄色基础色
        boxElement.classList.add("active-blinking");
        boxElement.title = `${box.name} (激活状态)`;
        
        // 添加闪动动画
        boxElement.style.animation = "blinkActive 1s infinite";
        boxElement.style.boxShadow = "0 0 8px rgba(255,255,0,1)";
      } else if (box.isCollected) {
        boxElement.style.backgroundColor = "#888"; // 灰色 - 已收集
        boxElement.style.boxShadow = "0 0 2px rgba(136,136,136,0.8)";
        boxElement.title = `宝箱: ${box.name} (已收集)`;
      } else {
        boxElement.style.backgroundColor = "#00f"; // 蓝色 - 未收集
        boxElement.style.boxShadow = "0 0 4px rgba(0,0,255,0.8)";
        boxElement.title = `宝箱: ${box.name} (未收集)`;
      }

      // 添加点击事件（可选）
      boxElement.addEventListener("click", function () {
        console.log(
          `点击了宝箱: ${box.name} at (${box.x}, ${box.y}), 已收集: ${box.isCollected}`
        );
      });

      mapContainer.appendChild(boxElement);
    });

    console.log("Treasure boxes updated:", {
      count: treasureBoxes.length,
      mapSize: [mapWidth, mapHeight],
      imgSize: [imgRect.width, imgRect.height],
      tileSize: [tileDisplayWidth, tileDisplayHeight],
    });
  }
}

// 显示地图Base64数据
window.showMapBase64 = showMapBase64;
function showMapBase64(base64Data) {
  const mapPreview = document.getElementById("map-preview");

  if (base64Data && base64Data.trim() !== "" && mapPreview) {
    // 有效的地图数据，设置图片状态
    window.hasMapImage = true;
    const dataUrl = "data:image/png;base64," + base64Data;
    mapPreview.src = dataUrl;

    // 当地图图片加载完成后重新计算玩家位置并更新显示
    mapPreview.onload = function () {
      // 更新地图容器显示状态（检查三个条件）
      updateMapContainerDisplay();

      // 如果有存储的玩家位置数据，重新计算位置
      setTimeout(function () {
        if (window.currentPlayerPosition) {
          updatePlayerPosition(
            window.currentPlayerPosition.relativeX,
            window.currentPlayerPosition.relativeY,
            window.currentPlayerPosition.mapX,
            window.currentPlayerPosition.mapY,
            window.currentPlayerPosition.mapWidth,
            window.currentPlayerPosition.mapHeight
          );
        }
      }, 100);
    };

    // 立即更新地图容器显示状态
    updateMapContainerDisplay();
  } else {
    // 没有地图数据或数据无效，清除图片状态并隐藏地图容器
    window.hasMapImage = false;
    hideMapContainer();
  }
}

// 隐藏地图容器并清理数据
window.hideMapContainer = hideMapContainer;
function hideMapContainer() {
  // 保存状态到localStorage
  window.mapContainerState = false;
  localStorage.setItem("mapContainerState", "false");

  // 清理地图图片状态
  window.hasMapImage = false;

  // 更新地图容器显示
  updateMapContainerDisplay();

  // 清理地图预览和玩家位置数据
  const mapPreview = document.getElementById("map-preview");
  const playerDot = document.getElementById("player-dot");
  const playerCoords = document.getElementById("player-coords");

  if (mapPreview) {
    mapPreview.src = "";
  }
  if (playerDot) {
    playerDot.style.display = "none";
  }
  if (playerCoords) {
    playerCoords.style.display = "none";
  }

  // 清除当前玩家位置数据
  window.currentPlayerPosition = null;
}