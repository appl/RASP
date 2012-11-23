#ifndef SCRIPT_H
#define SCRIPT_H

#include "script/Rule.h"
#include "script/Variable.h"

#include <vector>

class HWInput;
class HWOutput;
class ConfigManager;

class Script
{
public:
    Script();
    ~Script();

    static Script* load(std::string name);
    bool save();
    
    void setName(std::string str) { m_name = str;}
    std::string getName() const { return m_name;}
    void setDescription(std::string str) { m_desc = str;}
    std::string getDescription() const { return m_desc;}

    void addRule(Rule* rule);
    void addVariable(Variable* var);
    void clearVariables();

    void setConfig(ConfigManager* config); // used for Input and Output List
    std::list<HWInput*> getInputList() const;
    std::list<Variable*> getVariableList() const;
    std::list<HWOutput*> getOutputList() const;

    std::vector<Rule*> getRuleList() const;
    Rule* getRuleByName(std::string name) const;

    void init(ConfigManager* config);
    void deinit();

private:
    std::vector<Rule*> m_listRules;
    std::list<Variable*> m_listVars;

    std::string m_name;
    std::string m_desc;

    ConfigManager* m_config;

    friend class ScriptDialogTableModel;
};

#endif // SCRIPT_H
