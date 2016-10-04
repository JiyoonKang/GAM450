/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Paintable.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
/******************************************************************************/
/*!
\file  Paintable.h
\project  Bring the Paint
\author Esteban Maldonado Cabán

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma  once

#include "Graphics/includes/GraphicsSystem/Renderable.h"
#include "Graphics/includes/SkyeDX/Area.h"

#define DEFAULT_PAINT_TEXTURE_SIZE 128

class Paintable : public Core::GameComponent
{
public:
  Paintable();
  ~Paintable();

  virtual void Initialize() override;
  virtual void Remove() override;

  //Getters
  const std::string& GetPaintTextureName()const;
  const Renderable* GetRendOwner()const;
  const float* GetRawPaintData()const;
  const unsigned int GetRawDataSize()const;
  const unsigned int GetRawDataByteSize()const;
  const Skye::Area GetPaintTextureArea()const;
  const unsigned GetPaintTextureAreaWidth()const;
  const unsigned GetPaintTextureAreaHeight()const;

  //Setters
  void SetRendOwner(Renderable* rend);
  void SetPaintTextureName(const std::string& name);
  void SetCollisionColor(int x, int y, const float4 color);

  COMP_LUA_USABLE;

private:
  //Helper functions
  void ApplyPaintToData(int x, int y, const float4 color);

  Renderable* m_rendOwner;
  std::string m_paintTextureName;
  Skye::Area m_paintTextureArea;
  float* m_rawRBGAdata;
  unsigned int m_rawDataSize;
};