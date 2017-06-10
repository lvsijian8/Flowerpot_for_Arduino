
import java.io.*;
import java.net.*;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

public class ClientUI1 extends JFrame {
	JTextArea mainArea;

	JTextArea sendArea;

	ChatClient1 client;

	JTextField ipArea;

	JButton btnLink;

	public void setClient(ChatClient1 client) {
		this.client = client;
	}

	public ClientUI1() {
		super("客户端");
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
				client.sendMsg(sendArea.getText());
				mainArea.append(sendArea.getText() + "\n");
				sendArea.setText("");
			}
		});

		JPanel ipPanel = new JPanel();
		ipPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 10, 10));
		ipPanel.add(new JLabel("服务器："));
		ipArea = new JTextField(12);
		ipArea.setText("127.0.0.1");
		ipPanel.add(ipArea);
		btnLink = new JButton("连接");
		ipPanel.add(btnLink);

		btnLink.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				client = new ChatClient1(ipArea.getText().trim().toString(), ClientUI1.this);
				client.start();
				ClientUI1.this.setClient(client);
			}
		});

		panel.add(sendBtn, BorderLayout.EAST);
		panel.add(sendArea, BorderLayout.CENTER);

		contain.add(ipPanel, BorderLayout.NORTH);
		contain.add(mainAreaP, BorderLayout.CENTER);
		contain.add(panel, BorderLayout.SOUTH);
		setSize(300, 250);
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public static void main(String[] args) {
		ClientUI1 ui = new ClientUI1();
	}

}

class ChatClient1 extends Thread {
	Socket sc;

	BufferedReader in;

	PrintWriter out;

	ClientUI1 ui;

	public ChatClient1(String ip, ClientUI1 ui) {
		this.ui = ui;
		try {
			sc = new Socket(ip, 6606);//--------------------------------------------------------修改端口号
			System.out.println("已顺利联接到服务器。");
			ui.mainArea.append("[本客户端信息]已顺利联接到服务器。\n");
			out = new PrintWriter(sc.getOutputStream(), true);
			in = new BufferedReader(new InputStreamReader(sc.getInputStream()));
		} catch (Exception e) {
			System.out.println(e);
		}
	}

	public void run() {
		String msg = "";
		while (true) {
			try {
				msg = in.readLine();
			} catch (SocketException ex) {
				System.out.println(ex);
				break;
			} catch (Exception ex) {
				System.out.println(ex);
			}
			if (msg != null && msg.trim() != "") {
				System.out.println("" + msg);
				ui.mainArea.append(msg + "\n");
			}
		}
	}

	public void sendMsg(String msg) {
		try {
			out.println("" + msg);
		} catch (Exception e) {
			System.out.println(e);
		}
	}

}
