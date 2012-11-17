
#include "hw/HWOutputDCMotorI2C.h"
#include "hw/I2CThread.h"
#include "ConfigManager.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

HWOutputDCMotorI2C::HWOutputDCMotorI2C()
{
    m_slaveAddress = -1;
    m_i2cThread = NULL;
}

HWOutput* HWOutputDCMotorI2C::load(QDomElement *root)
{
    HWOutputDCMotorI2C* hw = new HWOutputDCMotorI2C();
    QDomElement elem = root->firstChildElement();

    while(!elem.isNull())
    {
        if( elem.tagName().toLower().compare("slaveaddress") == 0 )
        {
            hw->m_slaveAddress = elem.text().toInt();
        }
        elem = elem.nextSiblingElement();
    }

    // check for invalid parameters
    if( hw->m_slaveAddress > 127 || hw->m_slaveAddress < 0)
    {
        pi_warn("Invalid i2c parameters");
        return NULL;
    }

    return hw;
}

QDomElement HWOutputDCMotorI2C::save(QDomElement* root, QDomDocument* document)
{
    QDomElement output = HWOutputDCMotor::save(root, document);

    QDomElement type = document->createElement("hwtype");
    QDomText typeText = document->createTextNode("I2C");
    type.appendChild(typeText);

    output.appendChild(type);

    QDomElement slaveAddr = document->createElement("SlaveAddress");
    QDomText slaveAddrText = document->createTextNode(QString::number( m_slaveAddress ));
    slaveAddr.appendChild(slaveAddrText);

    output.appendChild(slaveAddr);

    return output;
}

void HWOutputDCMotorI2C::setI2C(int fd)
{
    int ret;
    unsigned char buf[2];

    if( ioctl(fd, I2C_SLAVE, m_slaveAddress) < 0)
    {
        pi_warn("Failed to talk to slave");
        return;
    }
    // first select register 0x00 on drv8830
    buf[0] = 0x00;

    // now set output state
    buf[1] = m_state | ((m_speed * 63 / 100) << 2);

    ret = write(fd, buf, 2);
    if(ret != 2)
    {
        pi_warn("Could not write to bus");
        return;
    }
}

void HWOutputDCMotorI2C::init(ConfigManager *config)
{
    m_i2cThread = config->getI2CThread();
}

void HWOutputDCMotorI2C::deinit()
{
    m_i2cThread = NULL;
}

void HWOutputDCMotorI2C::outputChanged()
{
    HWOutputDCMotor::outputChanged();

    m_i2cThread->addOutput(this);
}
