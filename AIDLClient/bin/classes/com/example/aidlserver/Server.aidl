package com.example.aidlserver;

interface Server {

    String getContent();

    String modifyContent(String org);
}