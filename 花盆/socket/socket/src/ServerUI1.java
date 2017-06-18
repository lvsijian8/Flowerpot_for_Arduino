
import java.io.*;
import java.net.*;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

public class ServerUI1 extends JFrame {
	final JTextArea mainArea;

	JTextArea sendArea;

	JTextField indexArea;

	MainServer server;

	public void setServer(MainServer server) {
		this.server = server;
	}

	public ServerUI1() {
		super("服务器端");
		Container contain = getContentPane();
		contain.setLayout(new BorderLayout());
		mainArea = new JTextArea();
		JScrollPane mainAreaP = new JScrollPane(mainArea);// 为文本区添加滚动条

		JPanel panel = new JPanel();
		panel.setLayout(new BorderLayout());
		sendArea = new JTextArea(3, 8);
		JButton sendBtn = new JButton("发送");

		sendBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				int index = Integer.parseInt(indexArea.getText()); // todo:
				// 从文本框获取index
				if (index == 0) {
					server.sendAll(sendArea.getText());
					mainArea.append("[群发]" + sendArea.getText() + "\n");
					sendArea.setText("");
				} else if (server.sendMsg(sendArea.getText(), index - 1)) {
					mainArea.append("[发往" + index + "]" + sendArea.getText()
							+ "\n");
					sendArea.setText("");
				}
			}
		});

		JPanel tmpPanel = new JPanel();
		indexArea = new JTextField(2);
		indexArea.setText("0");
		tmpPanel.add(sendBtn);
		tmpPanel.add(indexArea);

		panel.add(tmpPanel, BorderLayout.EAST);
		panel.add(sendArea, BorderLayout.CENTER);

		contain.add(mainAreaP, BorderLayout.CENTER);
		contain.add(panel, BorderLayout.SOUTH);
		setSize(300, 250);
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public static void main(String[] args) {
		ServerUI1 ui = new ServerUI1();
		MainServer server = new MainServer(ui);
	}

}

class MainServer {
	Socket[] threads;

	ThreadServer[] client = new ThreadServer[60];

	ServerSocket soc;

	int count;

	ServerUI1 ui;

	public MainServer(ServerUI1 ui) {
		this.ui = ui;
		ui.setServer(this);
		threads = new Socket[60];
		try {
			soc = new ServerSocket(6606);
		} catch (Exception e) {
			System.out.println(e);
		}
		count = 0;
		accept();
	}

	public void sendAll(String msg) {
		for (int i = 0; i < 60; i++)
			sendMsg(msg, i);
	}

	public boolean sendMsg(String msg, int i) {
		if (client[i] != null) {
			client[i].sendMsg(msg);
			return true;
		} else
			return false;
	}

	int searchIndex() {
		for (int i = 0; i < 60; i++)
			if (client[i] == null)
				return i;
		return -1;
	}

	public void accept() {
		try {
			while (true) {
				int i = searchIndex();
				if (i == -1)
					continue;
				threads[i] = soc.accept();
				client[i] = new ThreadServer(this, threads[i], ui, i);
				System.out.println("当前客户数：" + count++);
			}
		} catch (Exception e) {
			System.out.println(e);
		}
	}
}

// 可以和一个客户交互
class ThreadServer extends Thread {
	int port;

	Socket client;

	BufferedReader in;

	//PrintWriter out;

	OutputStream out;

	ServerUI1 ui;

	int index;

	MainServer server;

	public ThreadServer(MainServer server, Socket socket, ServerUI1 ui, int index) {
		this.server = server;
		this.index = index;
		this.ui = ui;
		this.port = 6606;
		this.client = socket;
		try {
			System.out.println("启动服务器成功，等待端口号：" + port);
			System.out.println("连接成功！来自" + client.toString());
			ui.mainArea.append("[服务器信息]启动服务器成功，等待端口号：" + port+"\n");
			ui.mainArea.append("[服务器信息]连接成功！来自" + client.toString()+"\n");
			in  = new BufferedReader(new InputStreamReader(client.getInputStream(), "utf-8"));
			//out = new PrintWriter(client.getOutputStream(), true);
			out = client.getOutputStream();
		} catch (Exception ex) {
			System.out.println(ex);
		}
		start();
	}

	public void run() {
		String msg = "";
		while (true) {
			try {
				msg = in.readLine();
			} catch (SocketException ex) {
				System.out.println(ex);
				server.client[index] = null;
				break;
			} catch (Exception ex) {
				System.out.println(ex);
			}
			if (msg != null && msg.trim() != "") {
				System.out.println("[客户" + index + "]" + msg);
				ui.mainArea.append("[客户" + index + "]" + msg + "\n");
			}
		}
	}

	public void sendMsg(String msg) {
		try {
			//out.println("[服务器]" + msg);
			out.write((msg+"\n").getBytes("utf-8"));
		} catch (Exception e) {
			System.out.println(e);
		}
	}
}
