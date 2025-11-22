//
//  HDBaseViewController.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/6/22.
//

import UIKit

import Toast_Swift

class HDBaseViewController: UIViewController {
    func purchaseSuccessCompele(_ productId: String) {
        
    }
    
    func showIAPView(_ productId: String) {
        let buyAction = UIAlertAction(title: NSLocalizedString("立即购买", comment: ""), style: .default) { _ in
            self.purchaseProduct(productId)
        }
        let nextAction = UIAlertAction(title: NSLocalizedString("下次一定", comment: ""), style: .cancel) { _ in
            
        }
        let restoreAction = UIAlertAction(title: NSLocalizedString("恢复购买", comment: ""), style: .default) { _ in
            self.restoreProduct(productId)
        }
        var title = HDAppsTool.doubleExpMessage().0
        var message = HDAppsTool.doubleExpMessage().1
        if productId == HDAppsTool.doubleGoldId() {
            title = HDAppsTool.doubleMoneyMessage().0
            message = HDAppsTool.doubleMoneyMessage().1
        }
        let alertController = UIAlertController(title: title, message: message, preferredStyle: .alert)
        alertController.addAction(buyAction)
        alertController.addAction(nextAction)
        alertController.addAction(restoreAction)
        self.present(alertController, animated: true, completion: nil)
    }
    
    private func purchaseProduct(_ productId: String) {
        view.makeBlockToastActivity()
        IAPManager.shared.purchaseProductWithId(productId: productId) { error in
            self.view.hideBlockToastActivity()
            if error == nil {
                debugPrint("successful purchase!")
                self.purchaseSuccess(productId)
            }
            else {
                debugPrint("something wrong.. \(error.debugDescription)")
                self.purchaseFaild()
            }
        }
    }
    
    private func restoreProduct(_ productId: String) {
        view.makeBlockToastActivity()
        IAPManager.shared.restoreCompletedTransactions { error in
            self.view.hideBlockToastActivity()
            if error == nil {
                debugPrint("successful purchase!")
                self.purchaseSuccess(productId)
            }
            else {
                debugPrint("something wrong.. \(error.debugDescription)")
                if let err = error as? NSError, err.code == -1 {
                    let message = NSLocalizedString("没有可恢复购买的商品，请选择立即解锁购买", comment: "")
                    self.purchaseFaild(message: message)
                    return
                }
                self.purchaseFaild()
            }
        }
    }
    
    private func purchaseSuccess(_ productId: String) {
        showConfettiView()
        
        var style = ToastStyle()
        style.messageAlignment = .center
        view.makeToast(NSLocalizedString("恭喜您，解锁了", comment: ""), duration:3, position: .center, style: style)
        
        purchaseSuccessCompele(productId)
    }
    
    func showConfettiView() {
        let confettiView = ConfettiView(frame: self.view.bounds)
        navigationController?.view.addSubview(confettiView)

        // Configure
        confettiView.config.particle = .confetti(allowedShapes: [.circle, .rectangle])

        // Start
        confettiView.start()

        DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
            // Stop
            confettiView.stop()
        }
    }
    
    func purchaseFaild(message: String? = nil) {
        let title = NSLocalizedString("哎呀，购买失败啦！", comment: "")
        var lMessage = NSLocalizedString("别担心，可能是网络或支付系统的小问题。请稍后重试，或检查您的支付方式是否正常。", comment: "")
        if let message = message {
            lMessage = message
        }
        let okAction = UIAlertAction(title: NSLocalizedString("稍后重试～", comment: ""), style: .default) { _ in
            
        }
        let alertController = UIAlertController(title: title, message: lMessage, preferredStyle: .alert)
        alertController.addAction(okAction)
        self.present(alertController, animated: true, completion: nil)
    }
}


extension UIView {
    func makeBlockToastActivity() {
        self.isUserInteractionEnabled = false
        makeToastActivity(.center)
    }
    
    func hideBlockToastActivity() {
        self.isUserInteractionEnabled = true
        hideToastActivity()
    }
}
