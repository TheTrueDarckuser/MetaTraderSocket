#include "pch.h"
#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _DLLAPI extern "C" __declspec(dllexport)
#define CURL_STATICLIB
#include <curl\curl.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <windows.h>
using namespace std;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

_DLLAPI int __stdcall WebReq(char &timestamp, char *predict)
{
    string postString = string(&timestamp);
    string readBuffer;

    CURL* curl_handle = curl_easy_init();
    if (curl_handle)
    {
        struct curl_slist* hs = NULL;
        hs = curl_slist_append(hs, "Content-Type: application/json");
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, hs);
        // задаем  url адрес
        curl_easy_setopt(curl_handle, CURLOPT_URL, "http://127.0.0.1/");
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postString.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, postString.length());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &readBuffer);
        // выполняем запрос
        CURLcode res = curl_easy_perform(curl_handle);

        // закрываем дескриптор curl
        curl_easy_cleanup(curl_handle);

        char predictData[50];
        readBuffer.copy(predict, readBuffer.length() + 1);
    }
    return(1);
}