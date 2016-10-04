/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Paintable.cpp 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch\precompiled.h"
#include "Graphics/includes/GraphicsSystem/Paintable.h"
#include "includes/GraphicsSystem/SkyeGraphics.h"
#include "Core/types/space/Space.h"

Paintable::Paintable() :m_rendOwner(nullptr), m_rawDataSize(0), m_rawRBGAdata(nullptr), m_paintTextureName("BanBan"), m_paintTextureArea(0,0)
{

}

Paintable::~Paintable()
{
  //delete the raw data because we don't need it anymore
  SafeDeleteArr(m_rawRBGAdata);
}

void Paintable::Initialize()
{
  //Get a pointer to its Renderable component...
  m_rendOwner = reinterpret_cast<Renderable*>(space->GetComponent(eRenderable, owner));

  //Do not even bother creating the texture if there's no one to assign it to
  if (!m_rendOwner)
    return;

  //Get the texture area from the owner
  m_paintTextureArea = GRAPHICS->GetTextureSize(m_rendOwner->GetTextureName());

  //if the renderable has no area just set the width and height to default size;
  if (m_paintTextureArea.width == 0 && m_paintTextureArea.height == 0)
    m_paintTextureArea.width = m_paintTextureArea.height = DEFAULT_PAINT_TEXTURE_SIZE;

  m_paintTextureArea.width = m_paintTextureArea.height = DEFAULT_PAINT_TEXTURE_SIZE;

  //then set the size to actual width * height * size of float
  m_rawDataSize = m_paintTextureArea.width * m_paintTextureArea.height * 4;

  //Allocate raw data for the texture
  m_rawRBGAdata = new float[m_rawDataSize];

  //report if not enough memory, and exit the function
  if (!m_rawRBGAdata)
  {
    ErrorIf(true, "ERROR: PAINT TEXTURE COULD NOT BE ALLOCATED");
    return;
  }

  //If successful, then zero everything out
  ZeroMemory(m_rawRBGAdata, m_rawDataSize * sizeof(float) );

  //test data - set hals of the texture blue
  for (unsigned int i = 2; i < m_rawDataSize / 2; i += 4)
    m_rawRBGAdata[i] = m_rawRBGAdata[i + 1] = 1.0f;

  //Get the name for the paint texture
  char buff[30];
  itoa(m_rendOwner->self.m_index, buff, 10);
  m_paintTextureName = std::string("PAINT_TEXTURE_") + std::string(buff);

  //Create the texture from the memory
  GRAPHICS->LoadTextureFromMemory(m_paintTextureName, m_rawRBGAdata, m_paintTextureArea, Skye::DataFormat::FLOAT4);

  //give the name of the paint texture to the renderable owner
  //m_rendOwner->SetPaintTexture(m_paintTextureName);
}

void Paintable::Remove()
{
}

const std::string& Paintable::GetPaintTextureName() const
{
  return m_paintTextureName;
}

const Renderable* Paintable::GetRendOwner() const
{
  return m_rendOwner;
}

void Paintable::SetRendOwner(Renderable* rend)
{
  m_rendOwner = rend;
}

void Paintable::SetPaintTextureName(const std::string& name)
{
  m_paintTextureName = name;
}

void Paintable::ApplyPaintToData(int x, int y, const float4 color)
{
  int index = y * m_paintTextureArea.width * 4;
  index += x * 4;

  m_rawRBGAdata[index] = color.x;
  m_rawRBGAdata[index + 1] = color.y;
  m_rawRBGAdata[index + 2] = color.z;
  m_rawRBGAdata[index + 3] = color.w;

}
const float* Paintable::GetRawPaintData() const
{
  return m_rawRBGAdata;
}

const unsigned int Paintable::GetRawDataSize() const
{
  return m_rawDataSize;
}

const unsigned int Paintable::GetRawDataByteSize() const
{
  return m_rawDataSize * sizeof(float);
}

void Paintable::SetCollisionColor(int x, int y, const float4 color)
{
  ApplyPaintToData(x, y, color);
}

const Skye::Area Paintable::GetPaintTextureArea() const
{
  return m_paintTextureArea;
}

const unsigned Paintable::GetPaintTextureAreaWidth() const
{
  return m_paintTextureArea.width;
}

const unsigned Paintable::GetPaintTextureAreaHeight() const
{
  return m_paintTextureArea.height;
}

COMP_LUA_DEP(Paintable);
