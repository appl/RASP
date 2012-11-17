
#include "ui/ScriptDialog.h"
#include "ui_ScriptDialog.h"

#include "ui/RuleDialog.h"


ScriptDialog::ScriptDialog(QWidget *parent, Script *script, ConfigManager* config) :
    QDialog(parent),
    ui(new Ui::ScriptDialog),
    m_model(this, script)
{
    ui->setupUi(this);
    m_script = script;
    m_script->setConfig(config);
    m_config = config;

    ui->listRules->setModel(&m_model);

    ui->editName->setText(QString::fromStdString(script->getName()));
    ui->editDesc->setText(QString::fromStdString(script->getDescription()));

    connect(ui->buttonAdd, SIGNAL(clicked()), this, SLOT(addRule()));
    connect(ui->buttonEdit, SIGNAL(clicked()), this, SLOT(editRule()));
    connect(ui->buttonDelete, SIGNAL(clicked()), this, SLOT(deleteRule()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
}

ScriptDialog::~ScriptDialog()
{
    delete this->ui;
}

void ScriptDialog::okPressed()
{
    m_script->setName( ui->editName->text().toStdString() );
    m_script->setDescription( ui->editDesc->text().toStdString() );
    // TODO: check m_name and m_desc

    this->done(Accepted);
}

void ScriptDialog::addRule()
{
    Rule* rule = new Rule();
    RuleDialog* dialog = new RuleDialog(this, rule, m_script);
    if( dialog->exec() == QDialog::Accepted )
    {
        m_script->addRule(rule);
        m_model.changed();
    }
    else
    {
        delete rule;
    }

    delete dialog;
}

void ScriptDialog::editRule()
{
    QModelIndexList indices = ui->listRules->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        RuleDialog* dialog = new RuleDialog(this, m_model.getRule(indices.front().row()), m_script);
        dialog->exec();
        m_model.changed();

        delete dialog;
    }
}

void ScriptDialog::deleteRule()
{
    QModelIndexList indices = ui->listRules->selectionModel()->selection().indexes();

    if(indices.size() != 0)
    {
        m_model.removeRow(indices.front().row());
    }
}
