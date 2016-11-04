package com.company;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

public class Main {

    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(1234);

            while(true) {
                Socket connectionSocket = serverSocket.accept();
                System.out.printf("ACCEPT");
                String clientMessage = (new Date()).toString();
                PrintWriter writer = new PrintWriter(connectionSocket.getOutputStream(), true);
                writer.println(clientMessage);
                connectionSocket.close();
            }
            //serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
