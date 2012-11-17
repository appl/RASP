#ifndef VARIABLEFRAME_H
#define VARIABLEFRAME_H

#include "script/Variable.h"
#include "script/VariableListener.h"

#include <QFrame>

namespace Ui {
    class VariableFrame;
}

class VariableFrame : public QFrame, public VariableListener
{
    Q_OBJECT
public:
    VariableFrame(Variable* variable);
    ~VariableFrame();

    Variable* getVariable() const { return m_variable;}

    void onVariableChanged(Variable* var);

signals:
    void onVariableChangedSignal();

private slots:
    void valueChanged(int value);
    void onVariableChangedGUI();

private:
    Ui::VariableFrame* ui;
    Variable* m_variable;
};

#endif // VARIABLEFRAME_H
