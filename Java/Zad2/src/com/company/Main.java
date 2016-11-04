package com.company;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

public class Main {

    private static final int SERVER_PORT = 1234;

    @SuppressWarnings("InfiniteLoopStatement")
    public static void main(String[] args) {
        try {
            final ServerSocket serverSocket = new ServerSocket(SERVER_PORT);
            while (true) {
                final Socket socket = serverSocket.accept();

                System.out.printf("New client\n");
                final String message = (new Date()).toString();
                final PrintWriter printWriter = new PrintWriter(socket.getOutputStream(), true);

                printWriter.println(message);
                socket.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
