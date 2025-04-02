/**
 * @file commandparser.h
 * @brief 命令解析器头文件
 * @author [欧阳尹明]
 * @version 1.0
 * @date 2025-4-02
 */

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QString>
#include <QMap>

/**
 * @brief 命令解析器类
 */
class CommandParser {
public:
    /**
     * @brief 解析用户输入的命令
     * @param command 命令字符串
     * @param params 解析后的参数
     * @return 解析是否成功
     */
    bool parseCommand(const QString& command, QMap<QString, QString>& params) {
        // 简单的命令解析，假设命令格式为 "cmd param1=value1 param2=value2"
        QStringList parts = command.split(" ");
        if (parts.isEmpty()) {
            return false;
        }
        QString cmd = parts.takeFirst();
        for (const QString& part : parts) {
            QStringList keyValue = part.split("=");
            if (keyValue.size() == 2) {
                params[keyValue[0]] = keyValue[1];
            }
        }
        return true;
    }
};

#endif // COMMANDPARSER_H

