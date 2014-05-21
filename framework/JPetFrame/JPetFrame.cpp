// JPet Frame - JPetFrame.cpp
#include "JPetFrame.h"


ClassImp(JPetFrame);


JPetFrame::JPetFrame(int p_id, 
		     std::string p_status, 
		     std::string p_description, 
		     int p_version,
		     JPetUser &p_JPetUser,
		     JPetLayer &p_JPetLayer, 
		     JPetSlot &p_JPetSlot) :
					    m_id(p_id),
					    m_status(p_status),
					    m_description(p_description),
					    m_version(p_version),
					    m_JPetUser(p_JPetUser),
					    m_JPetLayer(p_JPetLayer),
					    m_JPetSlot(p_JPetSlot)
{
}

JPetFrame::~JPetFrame()
{
}

int JPetFrame::id() const
{
  return m_id;
}

std::string JPetFrame::status() const
{
  return m_status;
}

std::string JPetFrame::description() const
{
  return m_description;
}

int JPetFrame::version() const
{
  return m_version;
}