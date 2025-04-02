// commandinputdialog.cpp
#include "commandinputdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

CommandInputDialog::CommandInputDialog(TrapezoidModeler& modeler, Extruder& extruder, QWidget *parent)
    : QDialog(parent), m_modeler(modeler), m_extruder(extruder) {


    // 创建命令输入和输出区域
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* inputLayout = new QHBoxLayout();
    m_commandEdit = new QTextEdit(this);
    m_executeButton = new QPushButton("Execute", this);
    connect(m_executeButton, &QPushButton::clicked, this, &CommandInputDialog::onExecuteCommandClicked);
    inputLayout->addWidget(m_commandEdit);
    inputLayout->addWidget(m_executeButton);
    mainLayout->addLayout(inputLayout);

    m_outputText = new QTextEdit(this);
    m_outputText->setReadOnly(true);
    mainLayout->addWidget(m_outputText);

    // 设置输入控件与输出控件的伸缩比例为 3:1
    mainLayout->setStretch(0, 3);
    mainLayout->setStretch(1, 1);
}

CommandInputDialog::~CommandInputDialog() {}

void CommandInputDialog::onExecuteCommandClicked() {
    QString command = m_commandEdit->toPlainText(); // 获取 QTextEdit 的文本
    QMap<QString, QString> params;
    if (m_parser.parseCommand(command, params)) {
        QString cmd = command.split(" ").first();
        m_visualizationWindow = new VisualizationWindow();
        if (m_handler.handleCommand(cmd, params, m_modeler, m_extruder, m_visualizationWindow)) {
            m_outputText->append("Command executed successfully.");
        } else {
            m_outputText->append("Command execution failed.");
        }
    } else {
        m_outputText->append("Invalid command format.");
    }
}


