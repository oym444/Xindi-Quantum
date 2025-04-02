/**
 * @file commandinputdialog.h
 * @brief 命令输入对话框头文件
 * @author [欧阳尹明]
 * @version 1.0
 * @date 2025-4-02
 */
#ifndef COMMANDINPUTDIALOG_H
#define COMMANDINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include "commandparser.h"
#include "commandhandler.h"
#include "trapezoidmodeler.h"
#include "extruder.h"
#include "visualizationwindow.h"

/**
 * @brief 命令输入对话框类
 */
class CommandInputDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param modeler 梯形建模器
     * @param extruder 拉伸器
     * @param parent 父控件
     */
    CommandInputDialog(TrapezoidModeler& modeler, Extruder& extruder, QWidget *parent = nullptr);
    /**
     * @brief 析构函数
     */
    ~CommandInputDialog();

private slots:
    /**
     * @brief 执行命令按钮点击事件
     */
    void onExecuteCommandClicked();

private:
    QTextEdit* m_commandEdit;      ///< 命令输入框
    QPushButton* m_executeButton;  ///< 执行按钮
    QTextEdit* m_outputText;       ///< 输出文本框
    CommandParser m_parser;        ///< 命令解析器
    CommandHandler m_handler;      ///< 命令处理器
    TrapezoidModeler& m_modeler;   ///< 梯形建模器引用
    Extruder& m_extruder;          ///< 拉伸器引用
    VisualizationWindow* m_visualizationWindow; ///< 可视化窗口
};
#endif // COMMANDINPUTDIALOG_H

