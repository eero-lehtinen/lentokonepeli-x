#pragma once

#include <TGUI\TGUI.hpp>
#include <SFML\Graphics.hpp>

class Master;
class Server;
class Client;

class GUI {
private:
	tgui::Gui gui;
	Master* master;

	tgui::Font font;
	
	sf::Clock clock;
	float time = 0;
	void initMainMenu();
	tgui::Panel::Ptr mainMenuPanel;
	tgui::EditBox::Ptr joinIpEditBox;
	tgui::EditBox::Ptr usernameEditBox;

	tgui::Label::Ptr pingLabel;
	tgui::Label::Ptr fpsLabel;

	void hostButtonPressed();
	void clientButtonPressed();

	bool serverUIInitalized;
	void initServer();
	void showServer();
	tgui::Panel::Ptr sPanel;
	tgui::ChatBox::Ptr sCLI;
	tgui::EditBox::Ptr sCLIInput;
	
	void onCLIInput();
	
	bool clientUIInitalized;
	void initClient();
	void showClient();
	tgui::Panel::Ptr cPanel;
	tgui::ChatBox::Ptr chatBox;
	tgui::EditBox::Ptr chatBoxInput;
	
	tgui::Panel::Ptr chooseTeamPanel;

	bool host;

public:
	void init(Master* master);
	void setTarget(sf::RenderTarget& target);
	void draw();
	void handleEvent(sf::Event event);

	void teamJoinAccepted();

	Server* server;
	Client* client;

	int lastPing = -1;
};