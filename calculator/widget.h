#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "button.h"
#include<QLineEdit>
#include<QStack>

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);

public slots:
    void digitClicked();
    void pointClicked();
    void changeClicked();
    void unaryOperatorClicked();
    void multiplicativeOperatorClicked();
    void addictiveOperatorClicked();
    void equalClicked();
    void backSpaceClicked();
    void clear();
    void clearAll();


private:
    Button* createButton(const QString& text,const char* member, const QColor &bgColor, const QColor &textColor);
    bool calculate(double rightOperand, const QString &pendingOperator);

private:
    
    QString pendingMultiplicativeOperator;
    QStack<double> operandStack;
    double factorSoFar;
    QString pendingAdditiveOperator;
    bool waitingForOperand;
    double sumSoFar;


    QLineEdit* display;
    Button* digitButtons[10];



};

#endif // WIDGET_H
