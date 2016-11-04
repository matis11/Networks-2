package com.company;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Socket;

public class Main {

    public static void main(String[] args) {
        final Socket clientSocket;
        try {
            clientSocket = new Socket("localhost", 1234);

            exerciseTwo(clientSocket);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void exerciseOne(Socket clientSocket) throws IOException {
        final BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        final String message = bufferedReader.readLine();
        System.out.printf(message);

        clientSocket.close();
    }

    private static void exerciseTwo(Socket clientSocket) throws IOException {
        final InputStream inputStream = clientSocket.getInputStream();
        final byte[] buf = new byte[30];

        if (inputStream.read(buf) != 0) {
            System.out.write(buf, 0, buf.length);
        }

        clientSocket.close();
    }
}
