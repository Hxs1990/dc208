//
//  LocalController.cpp
//  IF
//
//  Created by zhaohf on 13-8-30.
//
//

#include "LocalController.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../Ext/CCDevice.h"
//#include "support/user_default/CCUserDefault.h"
#include "TypeDefinition.h"
#include "CCCommonUtils.h"
#include "GlobalData.h"

USING_NS_CC;
USING_NS_CC_EXT;

const std::string localPath = "local/";
const std::string xmlFile = "database.local.xml";
const std::string iniFile = "text_zh_CN.ini";

LocalController* LocalController::m_LocalController = NULL;

LocalController::LocalController():m_objINIFileParser(NULL),m_objXMLParser(NULL),m_lang("")
{
    
}

LocalController::~LocalController()
{
    purgeData();
}

LocalController* LocalController::shared()
{
    if (!m_LocalController) {
        m_LocalController = new LocalController();
        m_LocalController->updatePath = CCFileUtils::sharedFileUtils()->getWritablePath();
        m_LocalController->init();
    }
    return m_LocalController;
}

std::string LocalController::getLanguageFileNameBasedOnUserSystem()
{
    std::string local = cocos2d::extension::CCDevice::getLanguage();
    CCLOG("test lang %s",local.c_str());
    if(local.find("zh_TW")<local.length()||local.find("zh-Hant")<local.length()){
        return "zh_TW";
    }
    if(local.find("zh_CN")<local.length()||local.find("zh-Hans")<local.length()){
        return "zh_CN";
    }
    
    if(local.find("en")<local.length()){
        return   "en";
    }
    
    if(local.find("pt")<local.length()){
        return  "pt";
    }
    if(local.find("tr")<local.length()){
        return  "tr";
    }
    if(local.find("fr")<local.length()){
        return  "fr";
    }
    if(local.find("no")<local.length()){
        return  "no";
    }
    if(local.find("ko")<local.length()){
        return  "ko";
    }
    if(local.find("ja")<local.length()){
        return  "ja";
    }
    if(local.find("nl")<local.length()){
        return  "nl";
    }
    if(local.find("it")<local.length()){
        return   "it";
    }
    if(local.find("de")<local.length()){
        return   "de";
    }
    if(local.find("es")<local.length()){
        return  "es";
    }
    if(local.find("th")<local.length()){
        return  "th";
    }
    if(local.find("ru")<local.length()){
        return  "ru";
    }
    if(local.find("it")<local.length()){
        return  "it";
    }
    if(local.find("nl")<local.length()){
        return  "nl";
    }
    if(local.find("pl")<local.length()){
        return  "pl";
    }
    if(local.find("ro")<local.length()){
        return  "ro";
    }
    if(local.find("fa")<local.length()){
        return  "fa";
    }
    return  "en";
}

std::string LocalController::getLanguageFileName()
{
    if (m_lang != "") {
        return m_lang;
    }
    std::string preferredLanguage = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PREFERRED_LANGUAGE,"");
    std::string fileName="";
    if(preferredLanguage==""){
        std::string analyticID = cocos2d::extension::CCDevice::getPublishChannel();
        if(analyticID == "cafebazaar"){
            fileName = "fa";
        }else{
            fileName = getLanguageFileNameBasedOnUserSystem();
        }
    }else{
        fileName=preferredLanguage;
    }

    bool fileExist = CCFileUtils::sharedFileUtils()->isFileExist("local/text_"+fileName+".ini");
    
    if (!fileExist)
    {
        fileName = "en";
    }
    
    m_lang = fileName;

    if (preferredLanguage != fileName) {
        CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PREFERRED_LANGUAGE, fileName);
        CCUserDefault::sharedUserDefault()->setStringForKey("current-version-code","");
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    return fileName;
}

void LocalController::init() {
    Autolock lock(g_platmMutex);
    purgeData();
    unsigned long ulc = clock();
    //成功下载xml的app版本
    std::string _xmlVersion = CCUserDefault::sharedUserDefault()->getStringForKey("current-app-version-code","");
    std::string _xmlQuestVersion = CCUserDefault::sharedUserDefault()->getStringForKey("current-version-code","");
    //当前APP版本
    std::string _appVersion = cocos2d::extension::CCDevice::getVersionName();
    //是否使用本地xml
    bool useLocalXml = false;
    //没有下载过xml，使用apk内xml
    CCLOG("XML version: %s", _xmlQuestVersion.c_str());
    
    if (_xmlVersion == "" || _xmlQuestVersion == "1.0.1487") {
        useLocalXml = true;
    }
    else//成功下载过xml
    {
        //成功下载xml的版本
        std::vector<std::string> _updateVersion;
        CCCommonUtils::splitString(_xmlVersion,".", _updateVersion);
        //当前app版本
        std::vector<std::string> _appXMLVersion;
        CCCommonUtils::splitString(_appVersion,".", _appXMLVersion);
        //如果app的版本大于成功下载xml版本，使用apk内xml
        if (_appXMLVersion[0] > _updateVersion[0] || _appXMLVersion[1] > _updateVersion[1] || _appXMLVersion[2] > _updateVersion[2]) {
            useLocalXml = true;
        }
    }
    
    std::string update1 = updatePath + xmlFile;
    std::string local1 = localPath + xmlFile;
    if (CCFileUtils::sharedFileUtils()->isFileExist(update1) && !useLocalXml) {
        try{
            m_objXMLParser = CCRapidXMLParser::parseWithFile(update1.c_str(),local1.c_str());
        }catch(exception e){
            std::string errorStr = "xmlError : ";
            std::string reason = e.what();
            CCLOG((errorStr + reason).c_str());
            m_objXMLParser = CCRapidXMLParser::parseWithFile(local1.c_str(),local1.c_str());
        }
    } else {
        m_objXMLParser = CCRapidXMLParser::parseWithFile(local1.c_str(),local1.c_str());
    }
    CCLOG("rapidxml load Time %lu", clock() - ulc);
    
    ulc = clock();
    std::string languageFileName = getLanguageFileName();
    
    std::string update2 = updatePath + "text_" + languageFileName + ".ini";
    std::string local2 = localPath + "text_" + languageFileName + ".ini";
    if (CCFileUtils::sharedFileUtils()->isFileExist(update2) && !useLocalXml) {
        try{
            m_objINIFileParser = CCINIParser::parseWithFile(update2.c_str(),local2.c_str());
        }catch(exception e){
            std::string errorStr = "INIError : ";
            std::string reason = e.what();
            CCLOG((errorStr + reason).c_str());
            m_objINIFileParser = CCINIParser::parseWithFile(local2.c_str(),local2.c_str());
        }
    } else {
        m_objINIFileParser = CCINIParser::parseWithFile(local2.c_str(),local2.c_str());
    }
    
    CCLOG("iniParser load Time %lu", clock() - ulc);
}

CCRapidXMLParser* LocalController::DBXMLManager()
{
    CCAssert(m_objXMLParser != NULL, "Must init before use the XML!");
    return m_objXMLParser;
    
//    {
//        //  从全局索引表中读取对象的属性
//        CCDictionary * pDictT = pXmlParser->getObjectByKey("1010101");
//        CCString *sValue = (CCString *)pDictT->objectForKey("x");
//        CCLOG("x = %s",sValue->m_sString.c_str());
    
        
//        pDictT = pXmlParser->getObjectByKey("929001");
//        sValue = (CCString *)pDictT->objectForKey("iso_x");
//        CCLOG("iso_x = %s",sValue->m_sString.c_str());
//        
//        pDictT = pXmlParser->getObjectByKey("10");
//        sValue = (CCString *)pDictT->objectForKey("rewardItem");
//        CCLOG("rewardItem = %s",sValue->m_sString.c_str());
//        
//        //  从分组索引表中读取对象的属性
//        CCDictionary * pGroupDict = pXmlParser->getGroupByKey("Item");
//        
//        CCDictionary * pDictItem = (CCDictionary *)pGroupDict->objectForKey("330005");
//        
//        sValue = (CCString *)pDictItem->objectForKey("cash_cost");
//        CCLOG("cash_cost = %s",sValue->m_sString.c_str());
        
//    }
    
}
CCINIParser* LocalController::TextINIManager()
{
    //TODO 根据手机语言取得不同ini文件
    CCAssert(m_objINIFileParser != NULL, "Must init before use the INI!");
    return m_objINIFileParser;
}

void LocalController::purgeData() {
    CC_SAFE_RELEASE_NULL(m_objINIFileParser);
    CC_SAFE_RELEASE_NULL(m_objXMLParser);
}
