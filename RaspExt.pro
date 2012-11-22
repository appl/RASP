# -------------------------------------------------
# Project created by QtCreator 2012-09-22T17:55:01
# -------------------------------------------------
QMAKE_CXXFLAGS += -std=c++0x
QT += xml
LIBS += -lbluetooth
TARGET = RaspExt
TEMPLATE = app
SOURCES += main.cpp \
    ui/MainWindow.cpp \
    ui/OutputRelaisFrame.cpp \
    ui/InputFaderFrame.cpp \
    ui/ScriptsTableModel.cpp \
    ui/ActionTableModel.cpp \
    ui/RuleDialog.cpp \
    ui/ScriptDialog.cpp \
    ui/InputButtonFrame.cpp \
    hw/HWInput.cpp \
    hw/HWInputFader.cpp \
    hw/HWInputButton.cpp \
    hw/HWOutput.cpp \
    util/Debug.cpp \
    hw/HWOutputRelais.cpp \
    ConfigManager.cpp \
    script/ConditionInputButton.cpp \
    script/Script.cpp \
    script/Rule.cpp \
    script/ConditionInput.cpp \
    script/ActionOutputRelais.cpp \
    script/ActionOutput.cpp \
    script/Action.cpp \
    script/Condition.cpp \
    ui/InputFrame.cpp \
    ui/OutputFrame.cpp \
    hw/HWInputButtonGPIO.cpp \
    ui/ScriptDialogTableModel.cpp \
    ui/ConditionDialog.cpp \
    script/ConditionInputFader.cpp \
    hw/HWInputFaderI2C.cpp \
    ui/VariableFrame.cpp \
    script/Variable.cpp \
    script/ConditionVariable.cpp \
    ui/OutputDCMotorFrame.cpp \
    hw/HWOutputDCMotor.cpp \
    ui/ConditionTableModel.cpp \
    ui/ActionDialog.cpp \
    script/ActionVariable.cpp \
    script/RuleTimerThread.cpp \
    script/ActionSleep.cpp \
    hw/HWOutputDCMotorI2C.cpp \
    hw/I2CThread.cpp \
    hw/GPIOInterruptThread.cpp \
    script/ActionCallRule.cpp \
    script/ActionOutputDCMotor.cpp \
    hw/PCF8575I2C.cpp \
    hw/HWInputButtonI2C.cpp \
    hw/HWOutputLED.cpp \
    script/ActionOutputLED.cpp \
    ui/OutputLEDFrame.cpp \
    hw/HWOutputLEDI2C.cpp \
    ui/OutputStepperFrame.cpp \
    hw/HWOutputStepper.cpp \
    ui/OutputStepperDetailsDialog.cpp \
    hw/HWOutputStepperI2C.cpp \
    script/ActionOutputStepper.cpp \
    script/ActionOutputStepperSoftStop.cpp \
    script/ActionOutputStepperRunVelocity.cpp \
    script/ActionOutputStepperSetPosition.cpp \
    util/Config.cpp \
    hw/BTThread.cpp \
    script/ActionOutputStepperSetParam.cpp
HEADERS += ui/MainWindow.h \
    ui/OutputRelaisFrame.h \
    ui/InputFaderFrame.h \
    ui/ScriptsTableModel.h \
    ui/ActionTableModel.h \
    ui/RuleDialog.h \
    ui/ScriptDialog.h \
    ui/InputButtonFrame.h \
    hw/HWInput.h \
    hw/HWInputFader.h \
    hw/HWInputButton.h \
    hw/HWInputListener.h \
    hw/HWOutput.h \
    hw/HWOutputListener.h \
    util/Debug.h \
    ConfigManager.h \
    ui/InputFrame.h \
    ui/OutputFrame.h \
    script/ConditionInput.h \
    script/Script.h \
    script/Rule.h \
    script/ConditionInput.h \
    script/Condition.h \
    script/ConditionInputButton.h \
    script/Action.h \
    script/ActionOutputRelais.h \
    script/ActionOutput.h \
    hw/HWInputButtonGPIO.h \
    ui/ScriptDialogTableModel.h \
    ui/ConditionDialog.h \
    script/ConditionInputFader.h \
    hw/I2CPolling.h \
    hw/HWInputFaderI2C.h \
    hw/I2COutput.h \
    script/Variable.h \
    ui/VariableFrame.h \
    script/VariableListener.h \
    script/ConditionVariable.h \
    ui/OutputDCMotorFrame.h \
    hw/HWOutputDCMotor.h \
    ui/ConditionTableModel.h \
    ui/ActionDialog.h \
    script/ActionVariable.h \
    script/RuleTimerThread.h \
    script/ActionSleep.h \
    hw/HWOutputDCMotorI2C.h \
    hw/I2CThread.h \
    hw/GPIOInterruptThread.h \
    util/Time.h \
    script/ActionCallRule.h \
    util/PriorityQueue.h \
    script/ActionOutputDCMotor.h \
    hw/PCF8575I2C.h \
    hw/HWInputButtonI2C.h \
    hw/HWOutputRelais.h \
    hw/HWOutputLED.h \
    script/ActionOutputLED.h \
    ui/OutputLEDFrame.h \
    hw/HWOutputLEDI2C.h \
    ui/OutputStepperFrame.h \
    hw/HWOutputStepper.h \
    ui/OutputStepperDetailsDialog.h \
    hw/HWOutputStepperI2C.h \
    script/ActionOutputStepperSoftStop.h \
    script/ActionOutputStepper.h \
    script/ActionOutputStepperRunVelocity.h \
    script/ActionOutputStepperSetPosition.h \
    util/Config.h \
    hw/BTThread.h \
    script/ActionOutputStepperSetParam.h
FORMS += ui/MainWindow.ui \
    ui/RuleDialog.ui \
    ui/ScriptDialog.ui \
    ui/InputFaderFrame.ui \
    ui/OutputRelaisFrame.ui \
    ui/InputButtonFrame.ui \
    ui/ConditionDialog.ui \
    ui/VariableFrame.ui \
    ui/OutputDCMotorFrame.ui \
    ui/ActionDialog.ui \
    ui/OutputLEDFrame.ui \
    ui/OutputStepperFrame.ui \
    ui/OutputStepperDetailsDialog.ui \
    ui/VariableListDialog.ui

OTHER_FILES += \
    TODO.txt
