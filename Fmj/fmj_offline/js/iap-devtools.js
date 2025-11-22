/**
 * 内购和开发工具接口文件
 * 专门处理iOS内购物品发放和游戏开发工具功能
 */

console.log("📱 iap-devtools.js 开始加载...");

// ==================== iOS 内购接口 ====================

/**
 * iOS 内购接口封装（供 Swift 调用）
 * 同步方法，直接返回成功或失败结果
 * @param {string} productId - 产品ID
 * @returns {string} JSON 字符串格式的结果
 */
window.iOSGrantIAPItems = function (productId) {
  console.log(`📱 iOS 调用内购接口: ${productId}`);

  try {
    // 检查游戏引擎是否已准备好
    if (!window.grantIAPItems) {
      console.error("❌ 游戏引擎尚未准备好");
      return JSON.stringify({
        success: false,
        message: "游戏加载中，请稍后再试",
        action: "engine_not_ready",
      });
    }

    // 直接调用内购接口（同步调用，不使用 await）
    const result = window.grantIAPItems(productId);
    console.log("内购结果:", result);

    // 检查结果是否为 Promise（异步方法）
    if (result && typeof result.then === 'function') {
      console.warn("⚠️ grantIAPItems 返回了 Promise，但需要同步结果");
      return JSON.stringify({
        success: false,
        message: "内购接口异步执行中，请稍后再试",
        action: "async_not_supported",
      });
    }

    // 返回同步结果
    if (result && result.success !== undefined) {
      return JSON.stringify(result);
    } else {
      // 如果结果格式不正确，假设成功
      return JSON.stringify({
        success: true,
        message: "物品添加成功",
        action: "granted",
      });
    }

  } catch (error) {
    console.error("❌ iOS 内购接口调用失败:", error);
    return JSON.stringify({
      success: false,
      message: "添加物品失败: " + error.message,
      action: "error",
    });
  }
};

console.log("✅ iOSGrantIAPItems 函数已定义");

// ==================== 开发工具接口 ====================

/**
 * 检查游戏引擎是否准备就绪
 * @returns {boolean} 游戏引擎是否可用
 */
function isGameEngineReady() {
  // 检查游戏的 Player 类是否可用
  const PlayerClass =
    window["fmj.core"] &&
    window["fmj.core"].fmj &&
    window["fmj.core"].fmj.characters &&
    window["fmj.core"].fmj.characters.Player;

  return !!(
    PlayerClass &&
    PlayerClass.Companion &&
    PlayerClass.Companion.sGoodsList
  );
}

/**
 * 添加物品到游戏
 * @param {number} type - 物品类型 (1-14)
 * @param {number} index - 物品索引 (1-255)
 * @param {number} count - 物品数量
 * @returns {boolean} 是否添加成功
 */
window.addItemToGame = function (type, index, count) {
  try {
    // 检查游戏引擎是否准备好
    if (!isGameEngineReady()) {
      console.error("❌ 游戏引擎尚未准备好");
      return false;
    }

    // 直接调用游戏的添加物品方法
    const PlayerClass = window["fmj.core"].fmj.characters.Player;
    const goodsList = PlayerClass.Companion.sGoodsList;
    const itemCount = count || 1;

    try {
      if (typeof goodsList.addGoods_qt1dr2$ === "function") {
        goodsList.addGoods_qt1dr2$(type, index, itemCount);
        console.log(
          `✅ 添加物品成功: 类型=${type}, 索引=${index}, 数量=${itemCount}`
        );
        return true;
      } else if (typeof goodsList.addGoods === "function") {
        goodsList.addGoods(type, index, itemCount);
        console.log(
          `✅ 添加物品成功: 类型=${type}, 索引=${index}, 数量=${itemCount}`
        );
        return true;
      }
    } catch (addError) {
      console.warn(
        `⚠️ 添加物品失败 类型=${type}, 索引=${index}: ${addError.message}`
      );
    }

    console.error("❌ 无法添加物品");
    return false;
  } catch (error) {
    console.error("❌ addItemToGame 失败:", error);
    return false;
  }
};

/**
 * 批量添加物品（用于"全物品"内购）
 * @param {number} itemCount - 每种物品的数量（默认3）
 * @returns {object} 添加结果统计
 */
window.addAllItemsToGame = function (itemCount = 3) {
  try {
    // 检查游戏引擎是否准备好
    if (!isGameEngineReady()) {
      console.error("❌ 游戏引擎尚未准备好");
      return { success: false, successCount: 0, failCount: 0 };
    }

    let successCount = 0;
    let failCount = 0;

    console.log("开始批量添加所有物品...");

    // 遍历所有物品类型和索引
    let maxs = [15, 18, 15, 14, 8, 18, 32, 30, 27, 4, 15, 6, 1, 13]
    for (let type = 1; type <= 14; type++) {
      let max = maxs[type-1] + 10
      for (let index = 1; index <= max; index++) {
        try {
          if (window.addItemToGame(type, index, itemCount)) {
            successCount++;
          } else {
            failCount++;
          }
        } catch (error) {
          failCount++;
        }
      }
    }

    console.log(
      `✅ 批量添加完成! 成功: ${successCount} 个物品, 失败: ${failCount} 个`
    );

    return {
      success: true,
      successCount: successCount,
      failCount: failCount,
    };
  } catch (error) {
    console.error("❌ addAllItemsToGame 失败:", error);
    return {
      success: false,
      successCount: 0,
      failCount: 0,
      error: error.message,
    };
  }
};

/**
 * 检查玩家是否已拥有特定物品
 * @param {number} type - 物品类型
 * @param {number} index - 物品索引
 * @returns {number} 拥有的数量，0表示没有
 */
window.checkItemOwnership = function (type, index) {
  console.log(`🔍 检查物品拥有情况: 类型=${type}, 索引=${index}`);
  
  try {
    // 检查游戏引擎是否准备好
    if (!isGameEngineReady()) {
      console.error("❌ 游戏引擎尚未准备好");
      return 0;
    }

    // 获取物品列表
    const PlayerClass = window["fmj.core"].fmj.characters.Player;
    const goodsList = PlayerClass.Companion.sGoodsList;
    
    console.log(`📦 物品列表对象:`, goodsList);

    let count = 0;
    // 检查物品数量
    if (typeof goodsList.getGoodsNum_qt1dr2$ === "function") {
      count = goodsList.getGoodsNum_qt1dr2$(type, index) || 0;
      console.log(`📊 通过 getGoodsNum_qt1dr2$ 获取数量: ${count}`);
    } else if (typeof goodsList.getGoodsNum === "function") {
      count = goodsList.getGoodsNum(type, index) || 0;
      console.log(`📊 通过 getGoodsNum 获取数量: ${count}`);
    } else {
      console.error("❌ 未找到可用的获取物品数量方法");
      return 0;
    }

    console.log(`✅ 物品拥有数量: 类型=${type}, 索引=${index}, 数量=${count}`);
    return count;
  } catch (error) {
    console.error("❌ checkItemOwnership 失败:", error);
    return 0;
  }
};

/**
 * iOS 内购物品发放接口（直接添加物品）
 * 同步版本，直接检查并添加物品
 * @param {string} productId - 产品ID
 * @returns {object} 结果 {success: boolean, message: string, action: string}
 */
window.grantIAPItems = function (productId) {
  console.log(`📦 发放内购物品: ${productId}`);

  try {
    // 检查游戏引擎是否准备好
    if (!isGameEngineReady()) {
      return {
        success: false,
        message: "游戏引擎尚未准备好，请稍后再试",
        action: "engine_not_ready",
      };
    }

    let grantSuccess = false;
    let itemName = "";

    switch (productId) {
      case "com.harry.fmj.jiulongdaojians":
        // 九龙道剑 - Type 7 (武器类), Index 14
        console.log(`🗡️ 发放九龙道剑...`);
        grantSuccess = window.addItemToGame(7, 14, 3);
        itemName = "九龙道剑 x3";
        console.log(`📊 九龙道剑发放结果: ${grantSuccess ? '成功' : '失败'}`);
        break;

      case "com.harry.fmj.chaohaiyi":
        // 潮海衣 - Type 2 (衣类/护甲), Index 18
        console.log(`👘 发放潮海衣...`);
        grantSuccess = window.addItemToGame(2, 18, 3);
        itemName = "潮海衣 x3";
        console.log(`📊 潮海衣发放结果: ${grantSuccess ? '成功' : '失败'}`);
        break;

      case "com.harry.fmj.bawangzhong":
        // 霸王钟 - Type 6 (饰品类/法宝), Index 15
        console.log(`🔔 发放霸王钟...`);
        grantSuccess = window.addItemToGame(6, 15, 3);
        itemName = "霸王钟 x3";
        console.log(`📊 霸王钟发放结果: ${grantSuccess ? '成功' : '失败'}`);
        break;

      case "com.harry.fmj.allgoods":
        // 伏魔记全物品发放
        console.log(`📦 发放伏魔记全物品包...`);
        console.log(`✅ 开始发放伏魔记全物品...`);
        const resultFMJ = window.addAllItemsToGame(3);
        grantSuccess = resultFMJ.success;
        itemName = "伏魔记全物品套装";
        console.log(`📊 伏魔记全物品发放结果: ${grantSuccess ? '成功' : '失败'}`);
        break;

      case "com.harry.fmj.xkx.allgoods":
        // 侠客行全物品发放
        console.log(`🏴‍☠️ 发放侠客行全物品包...`);
        console.log(`✅ 开始发放侠客行全物品...`);
        const resultXKX = window.addAllItemsToGame(3);
        grantSuccess = resultXKX.success;
        itemName = "侠客行全物品套装";
        console.log(`📊 侠客行全物品发放结果: ${grantSuccess ? '成功' : '失败'}`);
        break;

      case "com.harry.fmj.cbzz.allgoods":
        // 赤壁之战全物品发放
        console.log(`⚔️ 发放赤壁之战全物品包...`);
        console.log(`✅ 开始发放赤壁之战全物品...`);
        const resultCBZZ = window.addAllItemsToGame(3);
        grantSuccess = resultCBZZ.success;
        itemName = "赤壁之战全物品套装";
        console.log(`📊 赤壁之战全物品发放结果: ${grantSuccess ? '成功' : '失败'}`);
        break;

      case "com.harry.fmj.jyqxz.allgoods":
        // 金庸群侠传全物品发放
        console.log(`🗡️ 发放金庸群侠传全物品包...`);
        console.log(`✅ 开始发放金庸群侠传全物品...`);
        const resultJYQXZ = window.addAllItemsToGame(3);
        grantSuccess = resultJYQXZ.success;
        itemName = "金庸群侠传全物品套装";
        console.log(`📊 金庸群侠传全物品发放结果: ${grantSuccess ? '成功' : '失败'}`);
        break;

      case "com.harry.fmj.yzcq.allgoods":
        // 一中传奇全物品发放
        console.log(`🏫 发放一中传奇全物品包...`);
        console.log(`✅ 开始发放一中传奇全物品...`);
        const resultYZCQ = window.addAllItemsToGame(3);
        grantSuccess = resultYZCQ.success;
        itemName = "一中传奇全物品套装";
        console.log(`📊 一中传奇全物品发放结果: ${grantSuccess ? '成功' : '失败'}`);
        break;

      case "com.harry.fmj.vip":
        // VIP全物品发放 - 通用处理，支持所有游戏mod
        console.log(`👑 VIP会员全物品发放...`);
        console.log(`✅ 开始为VIP会员发放当前游戏全物品...`);
        const resultVIP = window.addAllItemsToGame(3);
        grantSuccess = resultVIP.success;
        itemName = "VIP专享全物品套装";
        console.log(`📊 VIP全物品发放结果: ${grantSuccess ? '成功' : '失败'}`);
        break;

      default:
        console.error(`❌ 未知的产品ID: ${productId}`);
        return {
          success: false,
          message: "未知的产品",
          action: "error",
        };
    }

    if (grantSuccess) {
      return {
        success: true,
        message: `成功添加 ${itemName} 到当前存档！`,
        action: "granted",
      };
    } else {
      return {
        success: false,
        message: "添加物品失败，请重试",
        action: "error",
      };
    }
  } catch (error) {
    console.error(`❌ 发放内购物品失败: ${error.message}`);
    return {
      success: false,
      message: "系统错误，请重试",
      action: "error",
    };
  }
};

/**
 * 获取支持的内购产品列表
 * @returns {Array} 支持的产品ID列表
 */
window.getSupportedIAPProducts = function() {
  return [
    "com.harry.fmj.vip",                  // VIP全物品（通用）
    "com.harry.fmj.jiulongdaojians",      // 九龙道剑
    "com.harry.fmj.chaohaiyi",            // 潮海衣
    "com.harry.fmj.bawangzhong",          // 霸王钟
    "com.harry.fmj.allgoods",             // 伏魔记全物品
    "com.harry.fmj.xkx.allgoods",         // 侠客行全物品
    "com.harry.fmj.cbzz.allgoods",        // 赤壁之战全物品
    "com.harry.fmj.jyqxz.allgoods",       // 金庸群侠传全物品
    "com.harry.fmj.yzcq.allgoods"         // 一中传奇全物品
  ];
};