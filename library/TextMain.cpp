#define AUTO_DLLMAIN
#include <DynRPG/DynRPG.h>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cmath>

struct textObj
{
    int x;
    int y;
    int color;
    bool fixed;
    std::string txt;
    RPG::Image* img;
};

std::string parseText(std::string in);
typedef std::map<std::string, textObj> TextMap;

int picID;
TextMap drawList;

bool onDrawPicture(RPG::Picture* picture)
{
    if(picture->id == picID)
    {
        TextMap::iterator itr = drawList.begin();
        while(itr != drawList.end())
        {
            int x = itr->second.x;
            int y = itr->second.y;
            if(itr->second.fixed)
            {
                x -= RPG::map->getCameraX();
                y -= RPG::map->getCameraY();
            }
            itr->second.img->alpha = 255 - picture->transparency/100.0 * 255;
            RPG::screen->canvas->draw(x, y, itr->second.img);
            itr++;
        }
    }
    return true;
}

void onInitTitleScreen()
{
    TextMap::iterator itr = drawList.begin();
    while(itr != drawList.end())
    {
        TextMap::iterator tmp = itr;
        RPG::Image::destroy(itr->second.img);
        itr++;
        drawList.erase(tmp);
    }
}

bool onStartup(char *pluginName)
{
    std::map<std::string, std::string> configuration = RPG::loadConfiguration(pluginName);
    std::map<std::string, std::string>::iterator itr = configuration.find("PictureID");
    if(itr != configuration.end())
    {
        picID = atoi(itr->second.c_str());
    }
    else
        picID = 1;
    return true;
}

void onSaveGame(int id, void __cdecl(*savePluginData)(char*data,int length))
{
    std::string s;
    TextMap::iterator itr = drawList.begin();
    while(itr != drawList.end())
    {
        std::stringstream ss;
        ss << itr->second.x << "," << itr->second.y << ",";
        ss << itr->second.txt << "," << itr->second.color << ",";
        ss << itr->second.fixed << "," << itr->first << ",";
        s += ss.str();
        itr++;
    }
    savePluginData((char *)s.c_str(), s.size());
}

void onLoadGame(int id, char* data, int length)
{
    char* buffer;
    if(data == NULL) return;
    buffer = strtok(data, ",");
    while(buffer != NULL)
    {
        textObj obj;
        obj.x = atoi(buffer);
        buffer = strtok(NULL, ",");
        obj.y = atoi(buffer);
        buffer = strtok(NULL, ",");
        obj.txt = buffer;
        buffer = strtok(NULL, ",");
        obj.color = atoi(buffer);
        buffer = strtok(NULL, ",");
        obj.fixed = atoi(buffer);
        buffer = strtok(NULL, ",");
        obj.img = RPG::Image::create(obj.txt.size()*6 + 8, 16);
        obj.img->setSystemPalette();
        obj.img->useMaskColor = true;
        obj.img->drawText(0, 0, obj.txt, obj.color);
        drawList.insert(std::make_pair<std::string, textObj >(buffer, obj));
        buffer = strtok(NULL, ",");
    }
}

std::string formatText(std::string s)
{
    int size = 2;
    while(size < s.size() && s[size]!=']')size++;
    size -= 2;
    switch (s[0])
    {
        case 'x':
        {
            TextMap::iterator itr = drawList.find(s.substr(2, size));
            if(itr != drawList.end())
                return itr->second.txt;
            return s;
        }
        case 'n':
        {
            int n = atoi(parseText(s.substr(2, size)).c_str());
            return RPG::actors[n]->getName();
        }
        case 'v':
        {
            int n = atoi(parseText(s.substr(2, size)).c_str());
            std::stringstream ss;
            ss << RPG::variables[n];
            return ss.str();
        }
        case 'i':
        {
            int n = atoi(parseText(s.substr(2, size)).c_str());
            return RPG::getItemName(n);
        }
        case 'I':
        {
            int n = atoi(parseText(s.substr(2, size)).c_str());
            return RPG::getItemDescription(n);
        }
        case 't':
        {
            int n = atoi(parseText(s.substr(2, size)).c_str());
            return RPG::getSkillName(n);
        }
        case 'T':
        {
            int n = atoi(parseText(s.substr(2, size)).c_str());
            return RPG::getSkillDescription(n);
        }
        default:
            return s;
    }
}

std::string parseText(std::string in)
{
    std::string out;
    for(unsigned int i = 0; i < in.size(); i++)
    {
        if(in[i] == '\\')
        {
            out += formatText(in.substr(i + 1, in.size()));
            while(i < in.size() && in[i]!=']')i++;
            while(i + 1 < in.size() && in[i+1]==']')i++;
        }
        else
            out += in[i];
    }
    return out;
}

bool onComment( const char* text,
                const RPG::ParsedCommentData* 	parsedData,
                RPG::EventScriptLine* 	nextScriptLine,
                RPG::EventScriptData* 	scriptData,
                int 	eventId,
                int 	pageId,
                int 	lineId,
                int* 	nextLineId )
{
    std::string s = parsedData->command;

    if(s.compare("write_text") == 0)
    {
        TextMap::iterator itr = drawList.find(parsedData->parameters[0].text);
        if(itr == drawList.end())
        {
            textObj obj;
            obj.x = parsedData->parameters[1].number;
            obj.y = parsedData->parameters[2].number;
            obj.color = (parsedData->parameters[5].type == RPG::PARAM_NUMBER && parsedData->parametersCount >= 6)?parsedData->parameters[5].number:0;
            if(parsedData->parameters[4].type == RPG::PARAM_STRING)
            {
                std::string tmp = parsedData->parameters[4].text;
                obj.fixed = (!tmp.compare("fixed"))?true:false;
            }
            else
                obj.fixed = false;
            if(parsedData->parameters[3].type == RPG::PARAM_NUMBER)
            {
                std::stringstream ss;
                ss << parsedData->parameters[3].number;
                obj.txt = ss.str();
            }
            else
                obj.txt = parseText(parsedData->parameters[3].text);
            obj.img = RPG::Image::create(obj.txt.size()*6 + 8, 16);
            obj.img->setSystemPalette();
            obj.img->useMaskColor = true;
            obj.img->drawText(0, 0, obj.txt, obj.color);
            drawList.insert(std::make_pair<std::string, textObj >(parseText(parsedData->parameters[0].text), obj));
        }
        return false;
    }

    if(s.compare("append_text") == 0)
    {
        TextMap::iterator itr = drawList.find(parseText(parsedData->parameters[0].text));
        if(itr != drawList.end())
        {
            if(parsedData->parameters[1].type == RPG::PARAM_NUMBER)
            {
                std::stringstream ss;
                ss << parsedData->parameters[1].number;
                itr->second.txt += ss.str();
            }
            else
                itr->second.txt += parseText(parsedData->parameters[1].text);
            itr->second.img->free();
            itr->second.img->init(itr->second.txt.size()*6 + 8, 16);
            itr->second.img->drawText(0, 0, itr->second.txt, itr->second.color);
        }
        return false;
    }

    if(s.compare("change_text") == 0)
    {
        TextMap::iterator itr = drawList.find(parseText(parsedData->parameters[0].text));
        if(itr != drawList.end())
        {
            if(parsedData->parameters[1].type == RPG::PARAM_NUMBER)
            {
                std::stringstream ss;
                ss << parsedData->parameters[1].number;
                itr->second.txt = ss.str();
            }
            else
                itr->second.txt = parseText(parsedData->parameters[1].text);
            itr->second.color = (parsedData->parameters[2].type == RPG::PARAM_NUMBER)?parsedData->parameters[2].number:0;
            itr->second.img->free();
            itr->second.img->init(itr->second.txt.size()*6 + 8, 16);
            itr->second.img->drawText(0, 0, itr->second.txt, itr->second.color);
        }
        return false;
    }

    if(s.compare("change_position") == 0)
    {
        TextMap::iterator itr = drawList.find(parseText(parsedData->parameters[0].text));
        if(itr != drawList.end())
        {
            itr->second.x = parsedData->parameters[1].number;
            itr->second.y = parsedData->parameters[2].number;
        }
        return false;
    }


    if(s.compare("remove_text") == 0)
    {
        TextMap::iterator itr = drawList.find(parseText(parsedData->parameters[0].text));
        if(itr != drawList.end())
        {
            RPG::Image::destroy(itr->second.img);
            drawList.erase(itr);
        }
        return false;
    }

    if(s.compare("remove_all") == 0)
    {
        TextMap::iterator itr = drawList.begin();
        while(itr != drawList.end())
        {
            RPG::Image::destroy(itr->second.img);
            drawList.erase(itr);
            itr++;
        }
        return false;
    }

    if(s.compare("debug") == 0)
    {
        TextMap::iterator itr = drawList.begin();
        while(itr != drawList.end())
        {
            MessageBox(NULL, itr->first.c_str(), "textPlugin", MB_ICONINFORMATION);
            itr++;
        }
        return false;
    }

    return true;
}
