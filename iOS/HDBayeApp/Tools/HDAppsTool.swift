//
//  HDTools.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/6/18.
//

import UIKit
import CryptoKit
import Foundation

// MARK: - Debug Logging Utility
extension HDAppsTool {
    static func debugLog(_ message: String, file: String = #file, function: String = #function, line: Int = #line) {
        #if DEBUG
        let fileName = (file as NSString).lastPathComponent
        print("[\(fileName):\(line)] \(function): \(message)")
        #endif
    }
}

// MARK: - URL Opening Utility
extension HDAppsTool {
    static func openURL(_ urlString: String, completion: ((Bool) -> Void)? = nil) {
        guard let url = URL(string: urlString) else {
            completion?(false)
            return
        }
        
        if UIApplication.shared.canOpenURL(url) {
            UIApplication.shared.open(url, options: [:]) { success in
                completion?(success)
            }
        } else {
            completion?(false)
        }
    }
    
    static func openAppOrStore(scheme: String, storeURL: String) {
        let schemeURL = "\(scheme)://"
        if let url = URL(string: schemeURL), UIApplication.shared.canOpenURL(url) {
            UIApplication.shared.open(url, options: [:], completionHandler: nil)
        } else {
            openURL(storeURL)
        }
    }
}

enum HDAppName: Int {
    case none = 0
    case hdBayeApp = 1
    case hdFmjApp = 2
    case hdRelatreeApp = 3
}

class HDAppsTool: NSObject {
    static private var _hdAppName: HDAppName = .none
    
    static func setupApps() {
        _hdAppName = hdAppName()
    }
    
    static func hdAppName() -> HDAppName {
        if _hdAppName != .none {
            return _hdAppName
        }
        
        guard let bundleIdentifier = Bundle.main.bundleIdentifier else {
            return _hdAppName
        }
        if bundleIdentifier == "com.harry.sgbaye" {
            _hdAppName = .hdBayeApp
        }
        else if bundleIdentifier.contains("com.harry.fmj") {
            _hdAppName = .hdFmjApp
        }
        
        return _hdAppName
    }
    
    static func appStoreUrl(_ name: HDAppName = _hdAppName) -> String {
        if name == .hdBayeApp {
            return "itms-apps://itunes.apple.com/app/id6744382643"
        }
        else if name == .hdFmjApp {
            return "itms-apps://itunes.apple.com/app/id6747572873"
        }
        else if name == .hdRelatreeApp {
            return "itms-apps://itunes.apple.com/app/id6742101752"
        }
        return ""
    }
    
    static func title() -> String {
        if _hdAppName == .hdBayeApp {
            return "三国霸业"
        }
        else if _hdAppName == .hdFmjApp {
            return "伏魔记"
        }
        return ""
    }
    
    static func indexPageName() -> String {
        if _hdAppName == .hdBayeApp {
            return "choose.html"
        }
        else if _hdAppName == .hdFmjApp {
            return "index.html"
        }
        return ""
    }
    
    static func strategyPageName() -> String {
        if _hdAppName == .hdBayeApp {
            return "choose.html"
        }
        else if _hdAppName == .hdFmjApp {
            return "games/fmj/strategy.html"
        }
        return ""
    }
    
    static func offlineZipName() -> String {
        if _hdAppName == .hdBayeApp {
            return "baye-offline-2025110101"
        }
        else if _hdAppName == .hdFmjApp {
            return "fmj-offline-2025111501"
        }
        return ""
    }
    
    static func offlineDirName() -> String {
        if _hdAppName == .hdBayeApp {
            return "baye-offline"
        }
        else if _hdAppName == .hdFmjApp {
            return "fmj-offline"
        }
        return ""
    }
    
    static func expectedSHA256() -> String {
        if _hdAppName == .hdBayeApp {
            return ""
        }
        else if _hdAppName == .hdFmjApp {
            return ""
        }
        return ""
    }
    
    static func zipPassword() -> String {
        if _hdAppName == .hdBayeApp {
            return ""
        }
        else if _hdAppName == .hdFmjApp {
            return offlineZipName() + "-" + expectedSHA256()
        }
        return ""
    }
    
    static func googleBannerAdUnitID() -> String {
        if _hdAppName == .hdBayeApp {
            return "ca-app-pub-2742872567383576/2055030657"
        }
        else if _hdAppName == .hdFmjApp {
            return "ca-app-pub-2742872567383576/6170702777"
        }
        return ""
    }
    
    static func googlePageAdUnitID() -> String {
        if _hdAppName == .hdBayeApp {
            return "ca-app-pub-2742872567383576/4569427633"
        }
        else if _hdAppName == .hdFmjApp {
            return "ca-app-pub-2742872567383576/7378190274"
        }
        return ""
    }
}


extension HDAppsTool {
    static func doubleExpId() -> String {
        return "com.harry.fmj.doubleExp"
    }
    
    static func doubleGoldId() -> String {
        return "com.harry.fmj.doubleGold"
    }
    
    static func jiulongdaojianId() -> String {
        return "com.harry.fmj.jiulongdaojians"
    }
    
    static func chaohaiyiId() -> String {
        return "com.harry.fmj.chaohaiyi"
    }
    
    static func bawangzhongId() -> String {
        return "com.harry.fmj.bawangzhong"
    }
    
    static func allGoodsId() -> String {
        return "com.harry.fmj.allgoods"
    }
    
    static func jiulongdaojianMessage() -> String {
        return "武器-九龙道剑X3：攻击+150、防御+20、身法+20、灵力+24、幸运+25；全体攻击。"
    }
    
    static func chaohaiyiMessage() -> String {
        return "衣服-潮海衣X3：HP+50、MP+50、防御+100、身法+18、灵力+15、幸运+10；免疫毒乱封眠"
    }
    
    static func bawangzhongMessage() -> String {
        return "装饰-霸王钟X3：攻击+100、灵力-20、幸运-40；魔器之一，极为霸气的法宝。"
    }
    
    static func allGoodsMessage() -> String {
        return "伏魔记/伏魔记完美版-全物品X3：全武器、全衣服、全饰品、全头戴、全药品、全披肩X3"
    }
    
    static func allGoodsId_XKX() -> String {
        return "com.harry.fmj.xkx.allgoods"
    }
    
    static func allGoodsMessage_XKX() -> String {
        return "侠客行-全物品X3：全武器、全衣服、全饰品、全头戴、全药品、全披肩X3"
    }
    
    static func allGoodsId_CBZZ() -> String {
        return "com.harry.fmj.cbzz.allgoods"
    }
    
    static func allGoodsMessage_CBZZ() -> String {
        return "赤壁之战之乱世枭雄/谁与争峰-全物品X3：全武器、全衣服、全饰品、全头戴、全药品、全披肩X3"
    }
    
    static func allGoodsId_JYQXZ() -> String {
        return "com.harry.fmj.jyqxz.allgoods"
    }
    
    static func allGoodsMessage_JYQXZ() -> String {
        return "金庸群侠传-全物品X3：全武器、全衣服、全饰品、全头戴、全药品、全披肩X3"
    }

    static func allGoodsId_YZCQ() -> String {
        return "com.harry.fmj.yzcq.allgoods"
    }
    
    static func allGoodsMessage_YZCQ() -> String {
        return "一中传奇系列-全物品X3：全武器、全衣服、全饰品、全头戴、全药品、全披肩X3"
    }
  
    static func allGoodsId_VIP() -> String {
        return "com.harry.fmj.vip"
    }
    
    static func allGoodsMessage_VIP() -> String {
        return "永久VIP解锁所有购买内容（<3折）：包含所有游戏（包含后续新增）的经验/金币加成、全物品X3"
    }
    
    // MARK: - 三国霸业内购产品
    static func bayeForLoveId() -> String {
        return "com.harry.baye.forlove"
    }
  
    static func bayeForLoveTitle() -> String {
        return "为爱发电，并解锁所有超能力（一次性费用）"
    }
    
    static func bayeForLoveMessage() -> String {
        return "为爱发电支持开发者：感谢您的支持，让我们能够持续为您带来更好的游戏体验！"
    }
    
    static func doubleExpMessage() -> (String, String){
        let randIndex = Int(arc4random() % 5)
        let doubleExpMessages = [
            ("甩甩账本-购买五倍经验", "瞧瞧这经验条，比您钱包余额涨得还慢！人家吃颗五倍丹跟开了挂似的蹦级，您搁这跟妖怪玩 \"过家家\"，打十只怪升的级够塞牙缝不？"),
            ("举望远镜-购买五倍经验", "远处大佬都穿神装虐 BOSS 了，您还在新手村跟野猪互啄呢！五倍经验丹在商店晃悠成霓虹灯了，再犹豫下去，NPC 都要挂横幅 \"建议转行捡破烂\" 啦～"),
            ("拍肩拍肩-购买五倍经验", "兄弟，您这升级速度是开了老年模式吧？别人吃丹像坐火箭，您打怪像踩缝纫机 —— 经验条拧半天就冒个泡，妖怪看了都想给您捐点经验值！"),
            ("摇摇铃铛-购买五倍经验", "五倍经验限时甩卖啦！再不下手，您这等级怕是要跟新手村的树桩子拜把子了 —— 人家刷一轮副本升 3 级，您打三小时怪才够换五草鞋！"),
            ("摊手摊手-购买五倍经验", "知道为啥妖怪见您不跑吗？人家算准了您没五倍经验，打完都能抽空喝杯茶！赶紧买丹吧，别让经验条在地图上画 \"到此一游\" 了成不？"),
        ]
        return doubleExpMessages[randIndex]
    }
    
    static func doubleMoneyMessage() -> (String, String){
        let randIndex = Int(arc4random() % 5)
        let doubleMoneyMessages = [
            ("金币乞丐记-购买五倍金币", "（敲碗）别人开五倍金币哐哐捡元宝，您蹲草丛扒拉三小时，背包金币还没 NPC 零花多！"),
            ("丐帮帮主篇-购买五倍金币", "（甩破碗）五倍金币丹在商店闪成金矿了，您还搁这跟小怪讨饭 —— 三小时刷出的钱够买半根糖葫芦不？"),
            ("铜板收集狂-购买五倍金币", "（摊手）大佬开五倍金币逛地图像扫街，您蹲角落扒拉石子儿，金币数凑起来刚够给商店老板发红包！"),
            ("负号养成记-购买五倍金币", "（指背包）别人吃丹后金币雨下成瀑布，您打怪爆的钱能买根草 —— 再犹豫，商店老板要挂 \"谢绝赊账\" 牌啦！"),
            ("金币绝缘体-购买五倍金币", "（敲空碗）五倍金币丹都快打折成白菜了，您还在跟野怪玩 \"铜板拔河\"，背包金币数够买个空气不？"),
        ]
        return doubleMoneyMessages[randIndex]
    }
    
    static func shareText() -> String {
        let randIndex = Int(arc4random() % 5)
        let shareTexts = [
            "发现一款超有意思的宝藏应用！玩法新奇又上头，强烈推荐给大家试试！",
            "被这款应用狠狠拿捏了！各种惊喜小细节，越玩越上瘾，分享给我的宝子们！",
            "最近挖到一款超好玩的应用，一玩就停不下来！必须安利给你们，快来一起快乐～",
            "救命！怎么会有这么宝藏的应用！好玩到忘记时间，按头安利给你们！",
            "挖到宝了！这款应用真的超绝，好玩又解压，忍不住分享给你们，赶紧码住！"
        ]
        return shareTexts[randIndex]
    }
    
    static func shareUrl() -> String {
        if _hdAppName == .hdBayeApp {
            return "https://apps.apple.com/cn/app/relatree/id6744382643"
        }
        else if _hdAppName == .hdFmjApp {
            return "https://apps.apple.com/cn/app/relatree/id6747572873"
        }
        return ""
    }
}


extension HDAppsTool {
    static func relationApps() -> [String] {
        if _hdAppName == .hdBayeApp {
            return ["伏魔记", "人际图谱"]
        }
        else if _hdAppName == .hdFmjApp {
            return ["三国霸业", "人际图谱"]
        }
        return []
    }
    
    static func relationAppsScheme() -> [String] {
        if _hdAppName == .hdBayeApp {
            return ["hdfmj", "hdrelatree"]
        }
        else if _hdAppName == .hdFmjApp {
            return ["hdbaye", "hdrelatree"]
        }
        return []
    }
    
    static func relationAppsLink() -> [String] {
        if _hdAppName == .hdBayeApp {
            return [
                appStoreUrl(.hdFmjApp),
                appStoreUrl(.hdRelatreeApp),
            ]
        }
        else if _hdAppName == .hdFmjApp {
            return [
                appStoreUrl(.hdBayeApp),
                appStoreUrl(.hdRelatreeApp),
            ]
        }
        return []
    }
}
