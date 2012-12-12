
#include "ui/ConfigDialog.h"
#include "hw/HWInput.h"
#include "hw/HWOutput.h"
#include "hw/BTThread.h"
#include "util/Debug.h"

#include "hw/HWInputButton.h"
#include "hw/HWInputButtonI2C.h"
#include "hw/HWInputButtonBtGPIO.h"

#include "ui_ConfigBTDialog.h"
#include "ui_ConfigDialog.h"
#include "ui_ConfigInputDialog.h"

#include <QMessageBox>

ConfigDialog::ConfigDialog(QWidget *parent, std::string name) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    // only try to load config if the name is not empty
    // empty name means a new config
    if( !name.empty() )
        m_config.load(name);


    // fill in values
    ui->editName->setText( QString::fromStdString( m_config.getName() ) );


    // set table models
    m_inputTableModel = new ConfigInputTableModel(this, &m_config);
    ui->tableInputs->setModel(m_inputTableModel);
    ui->tableInputs->horizontalHeader()->setStretchLastSection(true);

    m_outputTableModel = new ConfigOutputTableModel(this, &m_config);
    ui->tableOutputs->setModel(m_outputTableModel);
    ui->tableOutputs->horizontalHeader()->setStretchLastSection(true);

    m_btThreadTableModel = new ConfigBTThreadTableModel(this, &m_config);
    ui->tableBtBoards->setModel(m_btThreadTableModel);
    ui->tableBtBoards->horizontalHeader()->setStretchLastSection(true);


    // connect all signals - slots
    connect(ui->buttonAddInput, SIGNAL(clicked()), this, SLOT(addInput()));
    connect(ui->buttonEditInput, SIGNAL(clicked()), this, SLOT(editInput()));
    connect(ui->buttonDeleteInput, SIGNAL(clicked()), this, SLOT(deleteInput()));
    connect(ui->buttonDeleteOutput, SIGNAL(clicked()), this, SLOT(deleteOutput()));
    connect(ui->buttonAddBt, SIGNAL(clicked()), this, SLOT(addBt()));
    connect(ui->buttonEditBt, SIGNAL(clicked()), this, SLOT(editBt()));
    connect(ui->buttonDeleteBt, SIGNAL(clicked()), this, SLOT(deleteBt()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

ConfigDialog::~ConfigDialog()
{
    delete this->ui;

    delete m_inputTableModel;
    delete m_outputTableModel;
    delete m_btThreadTableModel;
}

void ConfigDialog::addInput()
{
    ConfigInputDialog dialog(this);

    if( dialog.exec() == QDialog::Accepted)
    {
        m_inputTableModel->add( dialog.assemble() );
    }
}

void ConfigDialog::editInput()
{
    QModelIndexList indices = ui->tableInputs->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        int row = indices.front().row();

        ConfigInputDialog dialog(this);

        dialog.edit( m_inputTableModel->get(row) );

        if( dialog.exec() == QDialog::Accepted)
        {
            m_inputTableModel->modifyRow(row, dialog.assemble());
        }
    }
}

void ConfigDialog::deleteInput()
{
    QModelIndexList indices = ui->tableInputs->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        int row = indices.front().row();

        m_inputTableModel->removeRow(row);
    }
}

void ConfigDialog::deleteOutput()
{
    QModelIndexList indices = ui->tableOutputs->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        int row = indices.front().row();

        m_outputTableModel->removeRow(row);
    }
}

void ConfigDialog::addBt()
{
    ConfigBTDialog dialog(this);

    if( dialog.exec() == QDialog::Accepted)
    {
        m_btThreadTableModel->add( dialog.assemble() );
    }
}

void ConfigDialog::editBt()
{
    QModelIndexList indices = ui->tableBtBoards->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        int row = indices.front().row();

        ConfigBTDialog dialog(this);

        dialog.edit( m_btThreadTableModel->get(row) );

        if( dialog.exec() == QDialog::Accepted)
        {
            m_btThreadTableModel->modifyRow(row, dialog.assemble());
        }
    }
}

void ConfigDialog::deleteBt()
{
    QModelIndexList indices = ui->tableBtBoards->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        int row = indices.front().row();

        m_btThreadTableModel->removeRow(row);
    }
}

void ConfigDialog::okPressed()
{
    if( ui->editName->text().size() == 0 )
    {
        // name is empty, we cannot save a script with an empty name
        QMessageBox(QMessageBox::Warning,
                    "Empty Name",
                    "The name of a config cannot be empty! Please fill in a name",
                    QMessageBox::Ok,
                    this).exec();
        return;
    }

    m_config.save();

    this->done(Accepted);
}





ConfigInputTableModel::ConfigInputTableModel(QObject *parent, Config *config) : QAbstractTableModel(parent)
{
    m_config = config;
}

int ConfigInputTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_config->m_listInput.size();
}

int ConfigInputTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ConfigInputTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= m_config->m_listInput.size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        HWInput* hw = *std::next(m_config->m_listInput.begin(), index.row());

        if(index.column() == 0)
            return QString::fromStdString( hw->getName() );
        else
            return QString::fromStdString( HWInput::HWInputTypeToString( hw->getType() ) );
    }

    return QVariant();
}

QVariant ConfigInputTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Name";
        case 1:
            return "Type";
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool ConfigInputTableModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row);

    // delete the variable, as it is no longer needed
    std::list<HWInput*>::iterator it = std::next(m_config->m_listInput.begin(), row);
    delete *it;

    m_config->m_listInput.erase(it);

    endRemoveRows();

    return true;
}

HWInput* ConfigInputTableModel::get(int row) const
{
    return *std::next(m_config->m_listInput.begin(), row);
}

void ConfigInputTableModel::add(HWInput *hw)
{
    if(hw != NULL)
    {
        beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());

        m_config->m_listInput.push_back(hw);

        endInsertRows();
    }
    else
    {
        pi_warn("Received null argument");
    }
}

void ConfigInputTableModel::modifyRow(int row, HWInput *hw)
{
    std::list<HWInput*>::iterator it = std::next(m_config->m_listInput.begin(), row);
    HWInput* old = *it;

    if(hw != old)
        delete old;

    *it = hw;

    emit dataChanged(this->index(row, 0), this->index(row, this->columnCount()));
}


ConfigOutputTableModel::ConfigOutputTableModel(QObject *parent, Config *config) : QAbstractTableModel(parent)
{
    m_config = config;
}

int ConfigOutputTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_config->m_listOutput.size();
}

int ConfigOutputTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ConfigOutputTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= m_config->m_listOutput.size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        HWOutput* hw = *std::next(m_config->m_listOutput.begin(), index.row());

        if(index.column() == 0)
            return QString::fromStdString( hw->getName() );
        else
            return QString::fromStdString( HWOutput::HWOutputTypeToString( hw->getType() ) );
    }

    return QVariant();
}

QVariant ConfigOutputTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Name";
        case 1:
            return "Type";
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool ConfigOutputTableModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row);

    // delete the variable, as it is no longer needed
    std::list<HWOutput*>::iterator it = std::next(m_config->m_listOutput.begin(), row);
    delete *it;

    m_config->m_listOutput.erase(it);

    endRemoveRows();

    return true;
}


ConfigBTThreadTableModel::ConfigBTThreadTableModel(QObject *parent, Config *config) : QAbstractTableModel(parent)
{
    m_config = config;
}

int ConfigBTThreadTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_config->m_listBTThread.size();
}

int ConfigBTThreadTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ConfigBTThreadTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= m_config->m_listBTThread.size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        BTThread* bt = *std::next(m_config->m_listBTThread.begin(), index.row());

        if(index.column() == 0)
            return QString::fromStdString( bt->getName() );
        else
            return QString::fromStdString( bt->getBTAddr() );
    }

    return QVariant();
}

QVariant ConfigBTThreadTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Name";
        case 1:
            return "Address";
        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool ConfigBTThreadTableModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row);

    // delete the variable, as it is no longer needed
    std::list<BTThread*>::iterator it = std::next(m_config->m_listBTThread.begin(), row);
    delete *it;

    m_config->m_listBTThread.erase(it);

    endRemoveRows();

    return true;
}

void ConfigBTThreadTableModel::add(BTThread *btThread)
{
    if(btThread != NULL)
    {
        beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());

        m_config->m_listBTThread.push_back(btThread);

        endInsertRows();
    }
    else
    {
        pi_warn("Received null argument");
    }
}

void ConfigBTThreadTableModel::modifyRow(int row, BTThread *btThread)
{
    std::list<BTThread*>::iterator it = std::next(m_config->m_listBTThread.begin(), row);
    BTThread* oldBt = *it;

    if(btThread != oldBt)
        delete oldBt;

    *it = btThread;

    emit dataChanged(this->index(row, 0), this->index(row, this->columnCount()));
}

BTThread* ConfigBTThreadTableModel::get(int row) const
{
    return *std::next(m_config->m_listBTThread.begin(), row);
}






ConfigBTDialog::ConfigBTDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigBTDialog)
{
    ui->setupUi(this);

    // connect all signals - slots
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));


    // TODO: check missing if bluetooth address is correct
}

ConfigBTDialog::~ConfigBTDialog()
{
    delete ui;
}

void ConfigBTDialog::edit(BTThread *btThread)
{
    ui->editName->setText( QString::fromStdString( btThread->getName() ) );
    ui->editAddr->setText( QString::fromStdString( btThread->getBTAddr() ) );
}

BTThread* ConfigBTDialog::assemble() const
{
    BTThread* btThread = new BTThread();

    btThread->setName( ui->editName->text().toStdString() );
    btThread->setBTAddr( ui->editAddr->text().toStdString() );

    return btThread;
}


ConfigInputDialog::ConfigInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigInputDialog)
{
    m_baseWidget = NULL;

    ui->setupUi(this);

    // connect all signals - slots
    connect(ui->comboType, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChanged(int)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    // default values
    this->comboChanged( ui->comboType->currentIndex() );
}

ConfigInputDialog::~ConfigInputDialog()
{
    if(m_baseWidget != NULL)
        delete m_baseWidget;

    delete ui;
}

void ConfigInputDialog::edit(HWInput *hw)
{
    ui->editName->setText( QString::fromStdString( hw->getName() ) );
    ui->comboType->setCurrentIndex( hw->getType() );

    if(m_baseWidget != NULL)
    {
        m_baseWidget->edit(hw);
    }
}

HWInput* ConfigInputDialog::assemble() const
{
    HWInput* hw = NULL;

    if(m_baseWidget != NULL)
        hw = m_baseWidget->assemble();

    hw->setName( ui->editName->text().toStdString() );

    return hw;
}

void ConfigInputDialog::comboChanged(int index)
{
    // delete old baseWidget if it exists
    if(m_baseWidget != NULL)
    {
        ui->gridLayout->removeWidget(m_baseWidget);
        delete m_baseWidget;
        m_baseWidget = NULL;
    }

    HWInput::HWInputType type = (HWInput::HWInputType)index;
    switch( type )
    {
    case HWInput::Button:
        m_baseWidget = new ConfigInputButtonWidget(this);
        break;
    case HWInput::Fader:
        break;
    }

    // add new baseWidget, this should never be NULL. If it is, somewhere something went wrong
    if(m_baseWidget != NULL)
    {
        ui->gridLayout->addWidget(m_baseWidget, 2, 0, 1, 2);
    }
}


ConfigInputButtonWidget::ConfigInputButtonWidget(QWidget *parent) : IConfigInputWidget(parent)
{
    QLabel* label = new QLabel("Select hardware", this);
    m_comboType = new QComboBox(this);
    m_comboType->addItem("Dummy");
    m_comboType->addItem("I2C");
    m_comboType->addItem("Bluetooth I2C");
    m_comboType->addItem("Bluetooth");

    m_labelBtBoard = new QLabel("Select bluetooth board", this);
    m_comboBtBoard = new QComboBox(this);

    m_labelI2CAddr = new QLabel("Select I2C address", this);
    m_spinI2CAddr = new QSpinBox(this);
    m_spinI2CAddr->setMinimum(0);
    m_spinI2CAddr->setMaximum(127);

    m_labelPort = new QLabel("Select pin", this);
    m_spinPort = new QSpinBox(this);
    m_spinPort->setMinimum(0);
    m_spinPort->setMaximum(15);


    QGridLayout* layout = new QGridLayout(this);

    // remove spacing around widget, it looks kind of odd otherwise
    layout->setContentsMargins(0, 0, 0, 0);

    // now add everything to the layout
    layout->addWidget(label, 0, 0);
    layout->addWidget(m_comboType, 0, 1);

    layout->addWidget(m_labelBtBoard, 1, 0);
    layout->addWidget(m_comboBtBoard, 1, 1);
    layout->addWidget(m_labelI2CAddr, 2, 0);
    layout->addWidget(m_spinI2CAddr, 2, 1);
    layout->addWidget(m_labelPort, 3, 0);
    layout->addWidget(m_spinPort, 3, 1);


    this->setLayout(layout);

    // now connect all signals - slots
    connect(m_comboType, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));

    // set default
    this->typeChanged( m_comboType->currentIndex() );
}

void ConfigInputButtonWidget::typeChanged(int index)
{
    HWInput::HWType type = (HWInput::HWType)index;
    switch(type)
    {
    case HWInput::Dummy:
        m_labelBtBoard->hide();
        m_comboBtBoard->hide();
        m_labelI2CAddr->hide();
        m_spinI2CAddr->hide();
        m_labelPort->hide();
        m_spinPort->hide();
        break;
    case HWInput::I2C:
        m_labelBtBoard->hide();
        m_comboBtBoard->hide();
        m_labelI2CAddr->show();
        m_spinI2CAddr->show();
        m_labelPort->show();
        m_spinPort->show();
        break;
    case HWInput::BtI2C:
        m_labelBtBoard->show();
        m_comboBtBoard->show();
        m_labelI2CAddr->show();
        m_spinI2CAddr->show();
        m_labelPort->show();
        m_spinPort->show();
        break;
    case HWInput::Bt:
        m_labelBtBoard->hide();
        m_comboBtBoard->hide();
        m_labelI2CAddr->hide();
        m_spinI2CAddr->hide();
        m_labelPort->show();
        m_spinPort->show();
        break;
    }
}

void ConfigInputButtonWidget::edit(HWInput *hw)
{
    m_comboType->setCurrentIndex( hw->getHWType() );

    switch(hw->getHWType())
    {
    case HWInput::I2C:
        m_spinI2CAddr->setValue( ((HWInputButtonI2C*)hw)->getSlaveAddress() );
        m_spinPort->setValue( ((HWInputButtonI2C*)hw)->getPort() );
        break;
    case HWInput::BtI2C:
        break;
    case HWInput::Bt:
        break;
    }
}

HWInput* ConfigInputButtonWidget::assemble()
{
    HWInputButton* hw = NULL;

    HWInput::HWType type = (HWInput::HWType)m_comboType->currentIndex();
    switch(type)
    {
    case HWInput::Dummy:
        hw = new HWInputButton();
        break;
    case HWInput::I2C:
        hw = new HWInputButtonI2C();
        ((HWInputButtonI2C*)hw)->setPort( m_spinPort->value() );
        ((HWInputButtonI2C*)hw)->setSlaveAddress( m_spinI2CAddr->value() );
        break;
    case HWInput::BtI2C:
        break;
    case HWInput::Bt:
        hw = new HWInputButtonBtGPIO();
        break;
    }

    return hw;
}
