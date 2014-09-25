//
//  Utils.cpp
//  超级拍拍
//
//  Created by zheng yu on 14-2-25.
//
//

#include "Utils.h"
#include <iconv.h>
namespace Utils
{

    int iconv_code(const std::string& fromcode, const std::string& tocode, const std::string& in, std::string& out)
    {
        char *sin, *sout;
        size_t lenin, lenout;
        int ret;
        iconv_t c_pt;
        
        if ((c_pt = iconv_open(tocode.c_str(), fromcode.c_str())) == (iconv_t)(-1))
        {
            out = in;
            return -1;
        }
        
        lenin = in.size()+1;//字符串要有个结束符位
        char* oldPasserName = new char[lenin];//这里oldPasserName为工作中用的名字，没什么特殊含义
        strcpy(oldPasserName, in.c_str());
        
        lenout = lenin*10;
        char* newPasserName = new char[lenout];
        
        sin = oldPasserName;
        sout = newPasserName;
        ret = iconv(c_pt, &sin, &lenin, &sout, &lenout);
        if(ret == -1)
        {
            out = in;
            iconv_close(c_pt);
            delete []oldPasserName;//别忘记释放掉
            delete []newPasserName;
            return -1;
        }
        iconv_close(c_pt);
        
        out = newPasserName;
        delete []oldPasserName;//别忘记释放掉
        delete []newPasserName;
        return 0;
    }
    
    std::string CharConversion::GBKToUTF8(const std::string& inStr)
    {
        std::string outStr;
        int result = iconv_code("GBK","UTF-8",inStr, outStr);
        if (result == 0)
        {
            return outStr;
        }
        return "";
    }

}

