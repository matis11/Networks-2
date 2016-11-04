package com.company;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Socket;

public class Main {

    public static void main(String[] args) {
        Socket clientSocket = null;
        try {
            clientSocket = new Socket("localhost", 1234);
            /*BufferedReader reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            String serverMessage = reader.readLine();
            System.out.printf(serverMessage);
            clientSocket.close();*/

            final InputStream is = clientSocket.getInputStream();
            final byte[] buffer = new byte[30];
            is.read(buffer);

            System.out.write(buffer, 0, buffer.length);
            clientSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
