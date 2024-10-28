#include "widget.h"
#include <QGridLayout>
#include <QFont>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget{parent}
{
    // 初始化状态
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    waitingForOperand = false;

    // 设置背景颜色为黑色
    QPalette p = this->palette();
    p.setColor(QPalette::Window, QColor(20, 20, 20));
    this->setAutoFillBackground(true);
    this->setPalette(p);

    // 创建显示屏
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 30);
    display->setFont(font);
    display->setMinimumHeight(100);

    QPalette displayPalette = display->palette();
    displayPalette.setColor(QPalette::Base, Qt::black);  // 背景黑色
    displayPalette.setColor(QPalette::Text, Qt::white);  // 字体白色
    display->setPalette(displayPalette);

    // 创建按钮
    for (int i = 0; i < 10; i++) {
        digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()), QColor(60, 60, 60), QColor(Qt::white)); // 浅黑色按钮，白色字体
    }

    Button* pointBtn = createButton(".", SLOT(pointClicked()), QColor(60, 60, 60), QColor(Qt::white));

    Button* backspaceBtn = createButton("⌫", SLOT(backSpaceClicked()), QColor(45, 45, 45), QColor(Qt::white));
    Button* clearBtn = createButton("CE", SLOT(clear()), QColor(45, 45, 45), QColor(Qt::white));
    Button* clearAllBtn = createButton("C", SLOT(clearAll()), QColor(45, 45, 45), QColor(Qt::white));

    Button* divBtn = createButton("÷", SLOT(multiplicativeOperatorClicked()), QColor(45, 45, 45), QColor(Qt::white));
    Button* timesBtn = createButton("×", SLOT(multiplicativeOperatorClicked()), QColor(45, 45, 45), QColor(Qt::white));
    Button* minusBtn = createButton("-", SLOT(addictiveOperatorClicked()), QColor(45, 45, 45), QColor(Qt::white));
    Button* plusBtn = createButton("+", SLOT(addictiveOperatorClicked()), QColor(45, 45, 45), QColor(Qt::white));

    Button* sqrtBtn = createButton("√", SLOT(unaryOperatorClicked()), QColor(45, 45, 45), QColor(Qt::white));
    Button* powerBtn = createButton("x²", SLOT(unaryOperatorClicked()), QColor(45, 45, 45), QColor(Qt::white));
    Button* reciprocalBtn = createButton("⅟x", SLOT(unaryOperatorClicked()), QColor(45, 45, 45), QColor(Qt::white));

    Button* equalBtn = createButton("=", SLOT(equalClicked()), QColor(0, 230, 230), QColor(60,60,60));

    Button* changeBtn = createButton("±", SLOT(changeClicked()), QColor(45, 45, 45), QColor(Qt::white));

    // 创建网格布局
    auto mainLayout = new QGridLayout(this);

    mainLayout->setSpacing(1);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // 显示屏在顶部，占据两行
    mainLayout->addWidget(display, 0, 0, 1, 4);

    mainLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding), 1, 0, 1, 4);
    mainLayout->setRowMinimumHeight(1, 10);

    // 布局按钮
    mainLayout->addWidget(changeBtn, 2, 0);
    mainLayout->addWidget(clearBtn, 2, 1);
    mainLayout->addWidget(clearAllBtn, 2, 2);
    mainLayout->addWidget(backspaceBtn, 2, 3);

    mainLayout->addWidget(reciprocalBtn, 3, 0);
    mainLayout->addWidget(powerBtn, 3, 1);
    mainLayout->addWidget(sqrtBtn, 3, 2);
    mainLayout->addWidget(divBtn, 3, 3);

    mainLayout->addWidget(digitButtons[7], 4, 0);
    mainLayout->addWidget(digitButtons[8], 4, 1);
    mainLayout->addWidget(digitButtons[9], 4, 2);
    mainLayout->addWidget(timesBtn, 4, 3);

    mainLayout->addWidget(digitButtons[4], 5, 0);
    mainLayout->addWidget(digitButtons[5], 5, 1);
    mainLayout->addWidget(digitButtons[6], 5, 2);
    mainLayout->addWidget(minusBtn, 5, 3);

    mainLayout->addWidget(digitButtons[1], 6, 0);
    mainLayout->addWidget(digitButtons[2], 6, 1);
    mainLayout->addWidget(digitButtons[3], 6, 2);
    mainLayout->addWidget(plusBtn, 6, 3);

    mainLayout->addWidget(digitButtons[0], 7, 0, 1, 2);
    mainLayout->addWidget(pointBtn, 7, 2);
    mainLayout->addWidget(equalBtn, 7, 3);

    // 设置布局
    setLayout(mainLayout);

    this->adjustSize();
}

// 处理数字输入
void Widget::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    int digitValue = clickedButton->text().toInt();

    if (display->text() == "0" && digitValue == 0)
        return;  // 防止多次输入 0

    if (waitingForOperand) {
        display->clear();  // 清空上次结果
        waitingForOperand = false;
    }
    if(display->text()== '0')
        display->setText(QString::number(digitValue));
    else
        display->setText(display->text() + QString::number(digitValue));
}

// 处理小数点输入
void Widget::pointClicked()
{
    if (waitingForOperand) {
        display->setText("0");
    }

    if (!display->text().contains('.')) {
        display->setText(display->text() + '.');
    }
    waitingForOperand = false;
}

// 处理退格键
void Widget::backSpaceClicked()
{
    if (waitingForOperand)
        return;

    QString text = display->text();
    text.chop(1);  // 删除最后一个字符
    if (text.isEmpty()) {
        text = "0";
        waitingForOperand = true;
    }
    display->setText(text);
}

// 处理CE键（清除当前操作数）
void Widget::clear()
{
    if(waitingForOperand)
        return;

    display->setText("0");
    waitingForOperand = true;
}

// 处理C键（全部清除）
void Widget::clearAll()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    display->setText("0");
    waitingForOperand = true;
}

// 处理加法、减法操作符
void Widget::addictiveOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();

    double operand = display->text().toDouble();

    if(!pendingMultiplicativeOperator.isEmpty())
    {
        if(!calculate(operand,pendingMultiplicativeOperator))
        {
            display->setText("错误");
            return;
        }
        display->setText(QString::number(factorSoFar));
        operand=factorSoFar;
        factorSoFar=0.0;
        pendingMultiplicativeOperator.clear();
    }

    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            display->setText("错误");
            return;
        }
        display->setText(QString::number(sumSoFar));
    } else {
        sumSoFar = operand;
    }

    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}

// 处理乘法、除法操作符
void Widget::multiplicativeOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
        if(!clickedButton)
            return;
        
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            display->setText("错误");
            return;
        }
        display->setText(QString::number(factorSoFar));
    } else {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;
}

// 处理等号
void Widget::equalClicked()
{
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            display->setText("错误");
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }

    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            display->setText("错误");
            return;
        }
        pendingAdditiveOperator.clear();
    } else {
        sumSoFar = operand;
    }

    display->setText(QString::number(sumSoFar));
    sumSoFar = 0.0;
    waitingForOperand = true;
}
// 处理单操作符，如平方根、倒数等
void Widget::unaryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();

    double operand = display->text().toDouble();
    double result = 0.0;

    if (clickedOperator == "√") {
        if (operand < 0.0) {
            display->setText("无效输入");
            return;
        }
        result = std::sqrt(operand);
    } else if (clickedOperator == "x²") {
        result = std::pow(operand, 2.0);
    } else if (clickedOperator == "⅟x") {
        if (operand == 0.0) {
            display->setText("除零错误");
            return;
        }
        result = 1.0 / operand;
    }

    display->setText(QString::number(result));
    waitingForOperand = true;
}

// 处理符号改变
void Widget::changeClicked()
{
    QString text = display->text();
    double value = text.toDouble();

    if (value != 0.0) {
        value = -value;
        display->setText(QString::number(value));
    }
}

// 辅助函数：计算
bool Widget::calculate(double rightoperand, const QString &operatorText)
{
    if (operatorText == "+") {
        sumSoFar += rightoperand;
    } else if (operatorText == "-") {
        sumSoFar -= rightoperand;
    } else if (operatorText == "\303\227") {
        factorSoFar *= rightoperand;
    } else if (operatorText == "\303\267") {
        if (rightoperand == 0.0)
            return false;
        factorSoFar /= rightoperand;
    }
    // display->setText(QString::number(sumSoFar));
    return true;
}
Button* Widget::createButton(const QString &text, const char *member, const QColor &bgColor, const QColor &textColor)
{
    Button* btn = new Button(text);  // 创建Button对象
    btn->setMinimumSize(100, 60);    // 设置按钮的最小大小

    // 增大按钮的字体大小
    QFont font = btn->font();
    font.setPointSize(font.pointSize() + 3);
    btn->setFont(font);

    // 设置按钮的背景颜色和字体颜色
    QPalette palette = btn->palette();
    palette.setColor(QPalette::Button, bgColor);  // 按钮背景颜色
    palette.setColor(QPalette::ButtonText, textColor);  // 按钮文本颜色
    btn->setPalette(palette);

    connect(btn, SIGNAL(clicked()), this, member);  // 将按钮的点击事件连接到槽函数
    return btn;
}


