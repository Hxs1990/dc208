//
//  TypeDefinition.cpp
//  IF
//
//  Created by guojiang on 2/3/16.
//
//

#include "TypeDefinition.h"
#include "CCTypesExt.h"
#include <string>

#define CHANGE_SERVER COCOS2D_DEBUG
//测试使用 不要提交
//#define CHANGE_SERVER 1
//#define CHANGE_SERVER 2

#define __SERVER__P1IP "p1.cok.elexapp.com"

// Server连接信息
#if CHANGE_SERVER == 1
    // 本地服
    #define __SERVER__IP "10.1.6.72"
    #define __SERVER__PORT 80
    #define __SERVER__ZONE "COK1"

    #define __SERVERLIST__IP "10.1.6.72"
    #define __GET__SERVERLIST__IP "10.1.6.72:8081"
#elif CHANGE_SERVER == 2
    // 测试服
    #define __SERVER__IP "169.45.155.123"
    #define __SERVER__PORT 80
    #define __SERVER__ZONE "COK"

    #define __SERVERLIST__IP "169.45.155.123"
    #define __GET__SERVERLIST__IP "p1.cok.elexapp.com"
#else
    // 正式服
    #define __SERVER__IP "s1.cok.elexapp.com"
    #define __SERVER__PORT 80
    #define __SERVER__ZONE "COK1"

    #define __SERVERLIST__IP "s1.cok.elexapp.com"
    #define __GET__SERVERLIST__IP "p1.cok.elexapp.com"
#endif

#define __SERVERLIST__CHINA_IP1 "p3cok.elexapp.com"
#define __SERVERLIST__CHINA_IP2 "p1cok.elexapp.com"

#define __SERVERLIST__1IP1 "184.173.110.102"
#define __SERVERLIST__1IP2 "184.173.110.99"
#define __SERVERLIST__IP1 "s1.cok.elexapp.com"
#define __SERVERLIST__IP2 "s2.cok.elexapp.com"

const char * getServerIP_TypeDefinition()
{
    return __SERVER__IP;
}

int getServerPort_TypeDefinition()
{
    return __SERVER__PORT;
}

const char * getServerZone_TypeDefinition()
{
    return __SERVER__ZONE;
}

const char * getServerListIP_TypeDefinition(int t)
{
    if (t == 0) return __SERVERLIST__IP;
    if (t == 1) return __SERVERLIST__CHINA_IP1;
    if (t == 2) return __SERVERLIST__CHINA_IP2;
    
    return __SERVERLIST__IP;
}

const char * get_getServerListIP_TypeDefinition()
{
    return __GET__SERVERLIST__IP;
}

const char * getServerAccountIP(int i)
{
    if (i == 1) return __SERVERLIST__1IP1;
    if (i == 2) return __SERVERLIST__IP2;
    
    return __SERVERLIST__1IP1;
}

const char * getServer(int i)
{
    if (i == 1) return __SERVERLIST__IP1;
    if (i == 2) return __SERVERLIST__IP2;
    
    return __SERVERLIST__IP1;
}

const char * getServerP1IP()
{
    return __SERVER__P1IP;
}

const char* getIPByType(int i)
{
    if (i == 0) return ".cok.elexapp.com";
    if (i == 1) return ".cok.elexcok.com";
    if (i == 2) return "lx.cok.elexapp.com";
    if (i == 3) return "tenp1.cok.elex.com";
    if (i == 4) return "hkp1.cok.elexapp.com";
    
    return ".cok.elexapp.com";
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const char* getPaymentCallbackURL(bool isTest)
{
    if (isTest) return "http://169.45.155.123:8080/gameservice/paymentcallback";
    else return "http://cnpay.cok.elexapp.com:8080/gameservice/paymentcallback";
}

const char* getPaymentRequestURL()
{
    return "http://cnpay.cok.elexapp.com:8080/gameservice";
}

const char * getAccountURL()
{
    return "http://cnac.cok.elexapp.com/account";
}
#endif

const char * getAnniversaryImageURL()
{
    // return "http://p1cok.elexapp.com:8088/gameservice/www/gateway/json.php";//测试url
    return "http://p1cok.elexapp.com/gameservice/www/gateway/json.php";
}

const char * getPostEventLogURL()
{
    // return "http://10.1.5.196:9000/client/loading";
    return "http://public.cok.elexapp.com/client/loading";
}

const char * getCustomPicURL()
{
    return "http://cok.eleximg.com/cok/img/";
}

const char * getFBLink()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "https://fb.me/827859737255348?feed_key=";
#else
    return "https://fb.me/789290781112244?feed_key=";
#endif
}

const char * getFBPShareLink()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "https://fb.me/789279541113368?from_feed=android_king";
#else
    return "https://fb.me/789290781112244?from_feed=ios_king";
#endif
}

const char* getFBIcon(int idx)
{
    if (idx == -1) return "http://cok.eleximg.com/cok/img/King_Feed_1.png";
    
    std::string ret("http://cok.eleximg.com/cok/img/fb_feed_0");
    ret.append(CC_ITOA(idx));
    ret.append(".png");
    return ret.c_str();
}

const char * getFBPictureSmallURL()
{
    return "https://graph.facebook.com/445208542288191/picture?type=small";
}

const char * getElexImgURL()
{
    return "http://cok.eleximg.com";
}

const char * getCollecDataURL()
{
    return "http://public.cok.elexapp.com/client/collecData";
}

/*

// 打开Facebook粉丝墙
void FBUtilies::fbShowFansWall(){
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://www.facebook.com/ClashOfKingsIF"]];
}
"https://www.facebook.com/Clash.Of.Kings.Game"
"https://fb.me/789290781112244?feed_key=ios_appInvite"
"http://cok.eleximg.com/cok/img/fb_share_icon1.png"
"https://www.facebook.com/Clash.Of.Kings.Game"
"http://news.qq.com/a/20140408/012675.htm"
"http://t.gdt.qq.com/conv/app/945274928/conv?%s"
"http://t.gdt.qq.com/conv/app/945274928/conv?v=%s&conv_type=MOBILEAPP_ACTIVITE&app_type=IOS&advertiser_id=552722"
"http://t.gdt.qq.com/conv/app/945274928/conv?%s"
"http://t.gdt.qq.com/conv/app/945274928/conv?v=%s&conv_type=MOBILEAPP_ACTIVITE&app_type=IOS&advertiser_id=1187663"
http://weibo.com/liewangdefenzheng
https://www.facebook.com/Clash-of-Kings-Espa%C3%B1ol-519397994874228/timeline/
https://m.facebook.com/Clash.Of.Kings.PT
https://www.facebook.com/Clash-of-Kings-Deutschland-1429065620745662/timeline/
"https://web.lobi.co/game/clash_of_kings"
https://vk.com/clashofking
http://weibo.com/liewangdefenzheng
http://ng.d.cn/coklwdfz/
http://cok.mi.com
http://cafe.naver.com/clashofkings

"https://www.facebook.com/871470146215610","https://twitter.com/ClashOfKingsCOK","http://www.youtube.com/user/ClashofKingsGame","http://vk.com/clashofking","http://cafe.naver.com/clashofkings","https://plus.google.com/communities/102750039774058615160","http://forum.337.com/en/forum-3760-1.html"

*/
