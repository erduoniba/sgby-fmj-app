// 全局编辑模式状态
window.portraitEditMode = false;

// 横屏模式缩放相关变量
// 初始化时尝试从localStorage恢复
(function() {
  const initScale = localStorage.getItem('landscape_scale');
  console.log('🔄 初始化时从localStorage读取的值:', initScale);

  // 总是优先使用localStorage中的值
  if (initScale && initScale !== 'null' && initScale !== 'undefined') {
    const parsedScale = parseFloat(initScale);
    if (!isNaN(parsedScale) && parsedScale > 0) {
      window.landscapeScale = parsedScale;
      console.log('✅ 初始化 landscapeScale 从localStorage:', window.landscapeScale);
    } else {
      window.landscapeScale = 1.0;
      console.log('⚠️ localStorage值无效，使用默认值:', window.landscapeScale);
    }
  } else {
    window.landscapeScale = 1.0;
    console.log('ℹ️ 初始化 landscapeScale 默认值:', window.landscapeScale);
  }
})();
window.landscapeMinScale = 0.6;
window.landscapeMaxScale = 2.0;

// 根据设备与视口动态调整横屏最小缩放（在平板上适当提高下限）
function updateLandscapeScaleBounds() {
  // 仅在横屏时生效
  if (isDevicePortrait()) return;

  const vw = window.innerWidth;
  const vh = window.innerHeight;
  const minSide = Math.min(vw, vh);

  // 简易平板判定：短边 >= 768 认为是平板
  const isTablet = minSide >= 768;
  if (isTablet) {
    window.landscapeMinScale = 2.0;
    window.landscapeMaxScale = 4.0;
  }
  else {
    window.landscapeMinScale = 0.6;
    window.landscapeMaxScale = 2.0;
  }

  applyLandscapeScale();
}

// 恢复竖屏模式下的拖拽位置
function restorePortraitPositions() {
  // 恢复app元素位置
  const appElement = document.getElementById('app');
  if (appElement) {
    const savedAppY = localStorage.getItem('portrait_app_y');
    if (savedAppY) {
      appElement.style.transform = `translateY(${savedAppY}px)`;
    }
  }
  
  // 恢复lcd元素位置
  const lcdElement = document.getElementById('lcd');
  if (lcdElement) {
    const savedLcdY = localStorage.getItem('portrait_lcd_y');
    if (savedLcdY) {
      lcdElement.style.transform = `translateY(${savedLcdY}px)`;
    }
  }
  
  // 恢复map-container元素位置
  const mapElement = document.getElementById('map-container');
  if (mapElement) {
    const savedMapY = localStorage.getItem('portrait_mapContainer_y');
    if (savedMapY) {
      mapElement.style.transform = `translateX(-50%) translateY(${savedMapY}px)`;
    } else {
      mapElement.style.transform = 'translateX(-50%)';
    }
  }
}

// 将函数暴露到全局作用域
window.restorePortraitPositions = restorePortraitPositions;

// 检查设备是否处于竖屏状态
function isDevicePortrait() {
  return window.innerHeight > window.innerWidth;
}

// 初始化编辑模式
function initPortraitEditMode() {
  // 只在设备竖屏时显示编辑按钮
  if (!isDevicePortrait()) {
    $('.portrait-edit-btn').hide();
    return;
  }
  
  $('.portrait-edit-btn').on('click', function() {
    window.portraitEditMode = !window.portraitEditMode;
    
    if (window.portraitEditMode) {
      $(this).addClass('active');
      $(this).html('✓');
      $('.draggable-element').addClass('edit-mode');
    } else {
      $(this).removeClass('active');
      $(this).html('⋮');
      $('.draggable-element').removeClass('edit-mode');
    }
  });
}

// 初始化拖拽功能
function initDraggable(element, storageKey) {
  // 只在设备竖屏时启用拖拽功能
  if (!isDevicePortrait()) {
    return;
  }

  // app元素在竖屏模式下不允许拖拽
  if (storageKey === 'app') {
    return;
  }
  
  // 移除之前的事件监听器（如果存在）
  element.removeEventListener('touchstart', element._handleStart);
  element.removeEventListener('mousedown', element._handleStart);
  document.removeEventListener('touchmove', element._handleMove);
  document.removeEventListener('mousemove', element._handleMove);
  document.removeEventListener('touchend', element._handleEnd);
  document.removeEventListener('mouseup', element._handleEnd);
  
  let isDragging = false;
  let startY = 0;
  let currentY = 0;
  
  // 对于 portrait-controls，使用 bottom 属性而不是 transform
  if (storageKey === 'portraitControls') {
    // 恢复保存的位置
    const savedBottom = localStorage.getItem(`portrait_${storageKey}_bottom`);
    if (savedBottom) {
      element.style.bottom = savedBottom + 'px';
    }
    
    let startBottom = 0;
    
    element.addEventListener('touchstart', handlePortraitControlsStart, { passive: false });
    element.addEventListener('mousedown', handlePortraitControlsStart);
    
    function handlePortraitControlsStart(e) {
      if (!window.portraitEditMode) return;
      
      const touch = e.touches ? e.touches[0] : e;
      isDragging = true;
      startY = touch.clientY;
      startBottom = parseInt(element.style.bottom || 0);
      element.classList.add('dragging');
      e.preventDefault();
    }
    
    document.addEventListener('touchmove', handlePortraitControlsMove, { passive: false });
    document.addEventListener('mousemove', handlePortraitControlsMove);
    
    function handlePortraitControlsMove(e) {
      if (!isDragging || !window.portraitEditMode) return;
      
      const touch = e.touches ? e.touches[0] : e;
      const deltaY = touch.clientY - startY;
      let newBottom = startBottom - deltaY;
      
      // 获取元素高度
      const elementHeight = element.offsetHeight;
      
      // 限制拖拽范围 - 确保元素不超出屏幕边界
      const minBottom = 0; // 底部不能低于屏幕底部
      const maxBottom = window.innerHeight - elementHeight - 10; // 顶部不能高于屏幕顶部，留10px边距
      
      newBottom = Math.max(minBottom, Math.min(maxBottom, newBottom));
      
      element.style.bottom = newBottom + 'px';
      e.preventDefault();
    }
    
    document.addEventListener('touchend', handlePortraitControlsEnd);
    document.addEventListener('mouseup', handlePortraitControlsEnd);
    
    function handlePortraitControlsEnd(e) {
      if (!isDragging) return;
      
      isDragging = false;
      element.classList.remove('dragging');
      
      // 保存位置
      localStorage.setItem(`portrait_${storageKey}_bottom`, parseInt(element.style.bottom || 0));
      e.preventDefault();
    }
    
    return; // 不执行下面的通用拖拽逻辑
  }
  
  // 其他元素的拖拽逻辑
  // 恢复保存的位置
  const savedY = localStorage.getItem(`portrait_${storageKey}_y`);
  if (savedY) {
    currentY = parseFloat(savedY);
    updateElementPosition(element, currentY);
  }
  
  function updateElementPosition(el, y) {
    if (storageKey === 'app') {
      // app 元素只需要设置 translateY，保持简单
      el.style.transform = `translateY(${y}px)`;
    } else if (storageKey === 'mapContainer') {
      // map-container 需要保持 translateX(-50%) 来居中
      el.style.transform = `translateX(-50%) translateY(${y}px)`;
    } else if (storageKey === 'lcd') {
      // lcd 元素类似 app 元素，只设置 translateY
      el.style.transform = `translateY(${y}px)`;
    } else {
      // 其他元素直接设置 translateY
      el.style.transform = `translateY(${y}px)`;
    }
  }
  
  function handleStart(e) {
    if (!window.portraitEditMode) return;
    
    const touch = e.touches ? e.touches[0] : e;
    isDragging = true;
    startY = touch.clientY - currentY;
    element.classList.add('dragging');
    e.preventDefault();
  }
  
  function handleMove(e) {
    if (!isDragging || !window.portraitEditMode) return;
    
    const touch = e.touches ? e.touches[0] : e;
    const newY = touch.clientY - startY;
    
    // 获取元素尺寸
    const elementRect = element.getBoundingClientRect();
    const elementHeight = elementRect.height;
    
    // 计算安全边距
    const safeMargin = 10;
    
    // 临时应用新位置来计算边界
    updateElementPosition(element, newY);
    const newRect = element.getBoundingClientRect();
    
    // 检查边界并调整
    let adjustedY = newY;
    
    // 如果顶部超出屏幕
    if (newRect.top < safeMargin) {
      adjustedY = newY + (safeMargin - newRect.top);
    }
    
    // 如果底部超出屏幕
    if (newRect.bottom > window.innerHeight - safeMargin) {
      adjustedY = newY - (newRect.bottom - (window.innerHeight - safeMargin));
    }
    
    currentY = adjustedY;
    updateElementPosition(element, currentY);
    e.preventDefault();
  }
  
  function handleEnd(e) {
    if (!isDragging) return;
    
    isDragging = false;
    element.classList.remove('dragging');
    
    // 保存位置
    localStorage.setItem(`portrait_${storageKey}_y`, currentY);
    e.preventDefault();
  }
  
  // 保存事件处理函数到元素上，以便后续移除
  element._handleStart = handleStart;
  element._handleMove = handleMove;
  element._handleEnd = handleEnd;
  
  // 绑定事件
  element.addEventListener('touchstart', handleStart, { passive: false });
  element.addEventListener('mousedown', handleStart);
  
  document.addEventListener('touchmove', handleMove, { passive: false });
  document.addEventListener('mousemove', handleMove);
  
  document.addEventListener('touchend', handleEnd);
  document.addEventListener('mouseup', handleEnd);
}

// 横屏模式下的缩放功能
function initLandscapeScaling() {
  // 只在横屏模式下启用
  if (isDevicePortrait()) {
    return;
  }

  const appElement = document.getElementById('app');
  const lcdElement = document.getElementById('lcd');

  if (!appElement || !lcdElement) {
    return;
  }

  // 从localStorage恢复缩放比例 - 每次切换到横屏都要恢复
  const savedScale = localStorage.getItem('landscape_scale');
  if (savedScale && savedScale !== 'null' && savedScale !== 'undefined') {
    const parsedScale = parseFloat(savedScale);
    if (!isNaN(parsedScale) && parsedScale > 0) {
      window.landscapeScale = parsedScale;
      console.log('✅ 恢复保存的缩放比例:', window.landscapeScale);
    } else {
      window.landscapeScale = 1.0;
      console.log('⚠️ 保存的缩放值无效，使用默认值 1.0');
    }
  } else {
    // 如果没有保存的缩放，使用默认值
    window.landscapeScale = window.landscapeScale || 1.0;
    console.log('ℹ️ 没有保存的缩放，使用默认缩放比例:', window.landscapeScale);
  }

  // 先根据设备与视口更新缩放上下限（平板会提高最小缩放）
  updateLandscapeScaleBounds();

  // 立即应用缩放，恢复上次的大小和位置
  console.log('🔄 应用横屏缩放...');
  applyLandscapeScale();

  // 变量来跟踪触摸状态
  let touchStartDistance = 0;
  let touchStartScale = window.landscapeScale;
  let lastTouchY = 0;
  let isDragging = false;
  let isPinching = false;

  // 处理双指缩放
  function handleTouchStart(e) {
    if (e.touches.length === 2) {
      isPinching = true;
      isDragging = false;

      // 计算两指之间的初始距离
      const touch1 = e.touches[0];
      const touch2 = e.touches[1];
      const dx = touch2.clientX - touch1.clientX;
      const dy = touch2.clientY - touch1.clientY;
      touchStartDistance = Math.sqrt(dx * dx + dy * dy);
      touchStartScale = window.landscapeScale;

      e.preventDefault();
    }
  }

  function handleTouchMove(e) {
    if (isPinching && e.touches.length === 2) {
      // 双指缩放
      const touch1 = e.touches[0];
      const touch2 = e.touches[1];
      const dx = touch2.clientX - touch1.clientX;
      const dy = touch2.clientY - touch1.clientY;
      const currentDistance = Math.sqrt(dx * dx + dy * dy);

      // 计算缩放比例
      const scaleFactor = currentDistance / touchStartDistance;
      let newScale = touchStartScale * scaleFactor;

      // 限制缩放范围
      newScale = Math.max(window.landscapeMinScale, Math.min(window.landscapeMaxScale, newScale));

      window.landscapeScale = newScale;
      console.log('📏 实时缩放值:', window.landscapeScale);
      applyLandscapeScale();

      e.preventDefault();
    }
  }

  function handleTouchEnd(e) {
    if (e.touches.length === 0) {
      isPinching = false;
      isDragging = false;

      // 保存缩放比例到localStorage
      console.log('💾 保存缩放比例到localStorage:', window.landscapeScale);
      localStorage.setItem('landscape_scale', window.landscapeScale);

      // 验证保存是否成功
      const verifyScale = localStorage.getItem('landscape_scale');
      console.log('✅ 验证保存的值:', verifyScale);
    }
  }

  // 移除之前的事件监听器（避免重复绑定）
  appElement.removeEventListener('touchstart', appElement._landscapeTouchStart);
  appElement.removeEventListener('touchmove', appElement._landscapeTouchMove);
  appElement.removeEventListener('touchend', appElement._landscapeTouchEnd);

  // 保存事件处理函数引用
  appElement._landscapeTouchStart = handleTouchStart;
  appElement._landscapeTouchMove = handleTouchMove;
  appElement._landscapeTouchEnd = handleTouchEnd;

  // 绑定事件 - 只绑定触摸事件
  appElement.addEventListener('touchstart', handleTouchStart, { passive: false });
  appElement.addEventListener('touchmove', handleTouchMove, { passive: false });
  appElement.addEventListener('touchend', handleTouchEnd, { passive: false });
}

// 应用横屏缩放
function applyLandscapeScale() {
  const appElement = document.getElementById('app');
  const lcdElement = document.getElementById('lcd');

  if (!appElement || !lcdElement) {
    console.log('❌ 找不到app或lcd元素');
    return;
  }

  // 确保是横屏模式
  if (isDevicePortrait()) {
    console.log('❌ 当前是竖屏模式，不应用横屏缩放');
    return;
  }

  // 确保缩放值有效
  if (!window.landscapeScale || isNaN(window.landscapeScale)) {
    console.log('⚠️ 缩放值无效，重置为1.0');
    window.landscapeScale = 1.0;
  }

  // 移除no-scale类，表示正在使用自定义缩放
  appElement.classList.remove('no-scale');
  console.log('✅ 移除no-scale类，应用自定义缩放');

  // 基础尺寸
  const baseWidth = 320;
  const baseHeight = 192;
  console.log(`📐 缩放比例: ${window.landscapeScale}`);

  // 使用transform scale实现统一缩放
  // 这样可以确保容器和内容完全同步缩放
  // 保持顶部对齐，左右居中
  const scaledHeight = baseHeight * window.landscapeScale;
  const topOffset = 0; // 顶部间距

  appElement.style.cssText = `
    width: ${baseWidth}px !important;
    height: ${baseHeight}px !important;
    position: absolute !important;
    top: ${topOffset}px !important;
    left: 50% !important;
    transform: translateX(-50%) scale(${window.landscapeScale}) !important;
    transform-origin: top center !important;
    margin: 0 !important;
    z-index: 1002 !important;
    background-color: #1a1a2e !important;
    border: 2px solid #4d80e4 !important;
    border-radius: 8px !important;
    box-shadow: 0 0 10px rgba(77, 128, 228, 0.6) !important;
    padding: 0 !important;
    overflow: hidden !important;
  `;

  // Canvas保持原始尺寸，让容器的scale来处理缩放，不需要单独边框
  lcdElement.style.cssText = `
    width: ${baseWidth}px !important;
    height: ${baseHeight}px !important;
    border: none !important;
  `;
  console.log('🎮 使用transform scale统一缩放');

  // 验证transform是否成功应用
  setTimeout(() => {
    const computedStyle = window.getComputedStyle(appElement);
    const transform = computedStyle.transform;
    console.log(`✅ Transform应用: ${transform}`);

    // 获取实际渲染的边界框（包含transform效果）
    const rect = appElement.getBoundingClientRect();
    console.log(`✅ 实际渲染尺寸: ${rect.width}x${rect.height}`);
    console.log('✅ 缩放应用成功！');
  }, 50);
  // const mapContainer = document.getElementById('map-container');
}

// 重置竖屏模式样式
function resetPortraitStyles() {
  const appElement = document.getElementById('app');
  const lcdElement = document.getElementById('lcd');

  if (!appElement || !lcdElement) {
    return;
  }

  // 清除所有横屏模式设置的内联样式
  appElement.style.width = '';
  appElement.style.height = '';
  appElement.style.position = '';
  appElement.style.top = '';
  appElement.style.left = '';
  appElement.style.transform = '';
  appElement.style.margin = '';
  appElement.style.marginTop = '';
  appElement.style.zIndex = '';
  appElement.style.backgroundColor = '';
  appElement.style.border = '';
  appElement.style.borderRadius = '';
  appElement.style.boxShadow = '';
  appElement.style.padding = '';
  appElement.style.overflow = '';

  // 清除canvas的内联样式
  lcdElement.style.width = '';
  lcdElement.style.height = '';

  // 添加no-scale类，让CSS媒体查询接管
  appElement.classList.add('no-scale');
}

// 初始化横屏缩放（需要在页面加载完成后调用）
function initLandscapeFeatures() {
  if (!isDevicePortrait()) {
    initLandscapeScaling();
  } else {
    // 竖屏模式下重置样式
    resetPortraitStyles();
  }
}

// 导出函数到全局
window.initLandscapeScaling = initLandscapeScaling;
window.applyLandscapeScale = applyLandscapeScale;
window.initLandscapeFeatures = initLandscapeFeatures;