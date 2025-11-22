//
//  HDWebViewController+Libs.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/7/17.
//

import UIKit

extension HDWebViewController: HDGameSelectionDelegate {

    // MARK: - HDGameSelectionDelegate
    func didSelectGame(key: String, value: String) {
        let script = """
        localStorage.setItem('choiceLibName', '\(key)');
        """
        wkWebView?.evaluateJavaScript(script) { (_, error) in
            if let error = error {
                debugPrint("切换游戏失败: \(error)")
            } else {
                debugPrint("切换游戏成功: \(value)")

                HDAppData.shared.choiceLib["key"] = key
                HDAppData.shared.choiceLib["value"] = value
                self.title = value
                self.changeLibReload()
            }
        }
    }

    // MARK: - Libs Management
    func refreshLibInfo() {
        let script = "window.libsList()"
        wkWebView?.evaluateJavaScript(script) { (result, error) in
            if let error = error {
                debugPrint("获取游戏列表失败: \(error)")
            }
            else {
                self.libsList = (result as? [String: String]) ?? [:]
                debugPrint("获取游戏列表成功：\(self.libsList)")
                
                // 保存到 HDAppData 供其他地方使用
                HDAppData.shared.libsList = self.libsList
                
                self.setupChangeLibButton()
            }
        }
    }
    
    func setupChangeLibButton() {
        changeLibButton = UIBarButtonItem(image: UIImage(systemName: "arrow.left.arrow.right"), style: .plain, target: self, action: #selector(changeLibButtonTapped))
        changeLibButton?.tintColor = .white
        navigationItem.leftBarButtonItem = changeLibButton
    }
    
    @objc func changeLibButtonTapped() {
        guard !libsList.isEmpty else { return }

        let gameSelectionVC = HDGameSelectionViewController()
        gameSelectionVC.libsList = libsList
        gameSelectionVC.currentTitle = title ?? ""
        gameSelectionVC.delegate = self

        let navigationController = UINavigationController(rootViewController: gameSelectionVC)
        navigationController.modalPresentationStyle = .pageSheet
        present(navigationController, animated: true)
    }
    
    private func changeLibReload() {
        wkWebView?.alpha = 0
        alreadyInjectJS = false
        wkWebView?.reload()
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.3) {
            UIView.animate(withDuration: 0.3) {
                self.wkWebView?.alpha = 1
            }
        }
    }
}
