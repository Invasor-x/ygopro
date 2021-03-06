#include "image_manager.h"

namespace ygo {

ImageManager imageManager;

bool ImageManager::Initial() {
	tCover[0] = driver->getTexture("textures/cover.jpg");
	tCover[1] = driver->getTexture("textures/cover2.jpg");
	tUnknown = driver->getTexture("textures/unknown.jpg");
	tAct = driver->getTexture("textures/act.png");
	tAttack = driver->getTexture("textures/attack.png");
	tChain = driver->getTexture("textures/chain.png");
	tNegated = driver->getTexture("textures/negated.png");
	tNumber = driver->getTexture("textures/number.png");
	tLPBar = driver->getTexture("textures/lp.png");
	tLPFrame = driver->getTexture("textures/lpf.png");
	tMask = driver->getTexture("textures/mask.png");
	tEquip = driver->getTexture("textures/equip.png");
	tTarget = driver->getTexture("textures/target.png");
	tLim = driver->getTexture("textures/lim.png");
	tHand[0] = driver->getTexture("textures/f1.jpg");
	tHand[1] = driver->getTexture("textures/f2.jpg");
	tHand[2] = driver->getTexture("textures/f3.jpg");
	tBackGround = driver->getTexture("textures/bg.jpg");
	tBackGround2 = driver->getTexture("textures/bg2.jpg"); 
	tField = driver->getTexture("textures/field2.png");
	tFieldTransparent = driver->getTexture("textures/field-transparent.png");
	return true;
}
void ImageManager::SetDevice(irr::IrrlichtDevice* dev) {
	device = dev;
	driver = dev->getVideoDriver();
}
void ImageManager::ClearTexture() {
	ScopedLock lk(mutex);
	for(auto tit = tMap.begin(); tit != tMap.end(); ++tit) {
		if(tit->second)
			driver->removeTexture(tit->second);
	}
	for(auto tit = tThumb.begin(); tit != tThumb.end(); ++tit) {
		if(tit->second)
			driver->removeTexture(tit->second);
	}
	tMap.clear();
	tThumb.clear();
}
void ImageManager::RemoveTexture(int code) {
	ScopedLock lk(mutex);
	auto tit = tMap.find(code);
	if(tit != tMap.end()) {
		if(tit->second)
			driver->removeTexture(tit->second);
		tMap.erase(tit);
	}
}
irr::video::ITexture* ImageManager::GetTexture(int code) {
	if(code == 0)
		return tUnknown;
	ScopedLock lk(mutex);
	auto tit = tMap.find(code);
	if(tit == tMap.end()) {
		char file[256];
		sprintf(file, "pics/%d.jpg", code);
		irr::video::ITexture* img = driver->getTexture(file);
		if(img == NULL) {
			tMap[code] = NULL;
			return tUnknown;
		} else {
			tMap[code] = img;
			return img;
		}
	}
	if(tit->second)
		return tit->second;
	else
		return tUnknown;
}
irr::video::ITexture* ImageManager::GetTextureThumb(int code) {
	if(code == 0)
		return tUnknown;
	ScopedLock lk(mutex);
	auto tit = tThumb.find(code);
	if(tit == tThumb.end()) {
		char file[256];
		sprintf(file, "pics/thumbnail/%d.jpg", code);
		irr::video::ITexture* img = driver->getTexture(file);
		if(img == NULL) {
			tThumb[code] = NULL;
			return GetTexture(code);
		} else {
			tThumb[code] = img;
			return img;
		}
	}
	if(tit->second)
		return tit->second;
	else
		return GetTexture(code);
}
irr::video::ITexture* ImageManager::GetTextureField(int code) {
	if(code == 0)
		return NULL;
	ScopedLock lk(mutex);
	auto tit = tFields.find(code);
	if(tit == tFields.end()) {
		char file[256];
		sprintf(file, "pics/field/%d.png", code);
		irr::video::ITexture* img = driver->getTexture(file);
		if(img == NULL) {
			tFields[code] = NULL;
			return NULL;
		} else {
			tFields[code] = img;
			return img;
		}
	}
	if(tit->second)
		return tit->second;
	else
		return NULL;
}

irr::video::ITexture* ImageManager::GetFieldTexture(int code, int player) {
	if(code == 0)
		return NULL;
	ScopedLock lk(mutex);
	int fieldcode = code + player;
	auto tit = tSpellFields.find(fieldcode);
	if(tit == tSpellFields.end()) {
		irr::video::ITexture* rt = 0;
		rt = driver->addRenderTargetTexture(core::dimension2d<u32>(512,512));
		driver->setRenderTarget(rt, false, false, video::SColor(0,0,0,255));
		ITexture *texture = imageManager.GetTextureField(code);
		if(texture)
			driver->draw2DImage(texture, irr::core::rect<s32>(0,0,512,512),player == 0 ? irr::core::rect<s32>(0,256,512,512) : irr::core::rect<s32>(0,0,512,256));
		driver->setRenderTarget(0, false, false, 0);
		tSpellFields[fieldcode] = rt;
		return rt;
	}

	if(tit->second)
		return tit->second;
	return NULL;
}
}