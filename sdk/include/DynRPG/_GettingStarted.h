/*!
	\page getting_started Getting Started

	\note This page describes how to get started with the <b>SDK</b>. If you
	want to know how to get started with the <b>Patch</b>, see \ref patch.

	\section sdk_preconditions The right C++ Compiler
	The DynRPG SDK works on a very low level. This means, small differences
	between compilers can break everything. The DynRPG was developed and tested
	using <a href="http://gcc.gnu.org/">GCC</a>, thus you should use GCC too.
	This DynRPG version only works with GCC version 4.7.1 and higher.
	You might use the <a href="http://www.codeblocks.org">Code::Blocks</a>
	IDE, for example.

	\section sdk_installation Installation
	Installing the SDK is easy - all you need to do is copying the files from
	the \c include and \c lib directories of the download archive into the
	corresponding folders of your compiler environment.

	\section sdk_header Header and library
	In order to use the DynRPG in your C++ project, you need to include
	<tt>DynRPG/DynRPG.h</tt>:
	\code
#include <DynRPG/DynRPG.h>
	\endcode
	\warning If you use more than one code module, you need to put the
	following line before the include statement in all modules except one:
	\code
#define NOT_MAIN_MODULE
	\endcode

	DynRPG automatically creates a \c DllMain function for you which stores
	the plugin's instance handle in a global variable called \c hInstance.
	You can suppress this behaviour by defining \c CUSTOM_DLLMAIN before
	including the header file.

	The header file automatically includes the headers \c string, \c map,
	\c windows.h because they are needed for the declarations.

	\warning Since the %RPG Maker is Non-Unicode, your plugin should be
	Non-Unicode too!

	You also need to add the library \c DynRPG (\c libDynRPG.a) to your
	project. See your IDE's help file if you don't know how to do this.
	(Normally you need to add the parameter <tt>-lDynRPG</tt> to the linker's
	command line.)

	\section sdk_callbacks Callbacks
	Together with the \ref sdk_game_objects, callbacks are important ways to
	interface your plugin with the game. Callbacks are functions which you can
	define in your plugin. On certain events in the game (for example, a
	picture is drawn - see \ref onDrawPicture) certain callbacks will be
	called by DynRPG. Callbacks either return \c void or \c bool. If you return
	\c false in \c bool callbacks, this will prevent other plugins from
	receiveing the same notification and sometimes it will also prevent some
	event from happening in the game. For example, if you return \c false from
	your \c onDrawPicture callback, the picture won't be drawn and other
	plugins (after yours) won't receive \c onDrawPicture for that picture.

	You only need to define those callback functions which you actually use.

	For a list of available callbacks, click here: \ref callbacks

	\section sdk_game_objects Game objects
	The game objects are the second important way to interface your plugin with
	the game. Most of the RPG classes are tied to a game object and should only
	be used through it. For example, the game object tied to RPG::Actor is
	called RPG::actors.

	Game objects represent certain "things" in the game. For example, the
	RPG::map object (the corresponding class is called RPG::Map) represents the
	current map. So, to get the width of the current map, you may call the
	RPG::Map::getWidth function like this:
	\code
int mapWidth = RPG::map->getWidth();
	\endcode

	For a list of available game objects, click here: \ref game_objects

	\section sdk_hello_world Hello World - Our first plugin
	It's time to create our first plugin. It's going to be <i>really</i>
	simple - all it is going to do is displaying a message box asking whether
	we really want to play before actually starting the game. We'll call
	it \c are_you_sure.

	First, create an %RPG Maker 2003 project to test the plugin and apply
	\ref patch to it. Then, create a new C++ project in your favorite IDE and
	select <b>DLL</b> as project type. It might be convinient to store the
	source code in a subfolder of the \c DynPlugins directory of your test
	project and set the compiler output directory to the \c DynPlugins
	directory.

	\warning If you are using Windows Vista or Windows 7, make sure you
	have write access to your project directory! You better put it into
	your \c Documents folder. Otherwise make sure you run both %RPG Maker
	2003 and your IDE (e.g. Code::Blocks) with administrator privileges!

	Okay, I'll be give a bit more step-by-step instructions now - of course
	you can do it differently if you know how, but this should help people who
	are not familiar with Code::Blocks to get started:
		<ol><li>Download Code::Blocks <a href="http://www.codeblocks.org/downloads/binaries">here</a>.
		Make sure you select the "bigger" package which contains MinGW which
		includes the GNU C++ compiler (which we need).</li>
		<li>Install Code::Blocks. It's okay if you don't change the default
		feature selection. Please remember or write down to path you are
		installing Code::Blocks to (e.g. <tt>C:\\Program Files (x86)\\CodeBlocks</tt>)!</li>
		<li>Open the path where you installed Code::Blocks in Windows Explorer.</li>
		<li>There should be a subfolder called \c MinGW. Open it. There should be folders
		called \c include and \c lib inside, among others.</li>
		<li>Copy the \c include and \c lib folders from your DynRPG download
		(in the \c sdk subfolder) into the \c MinGW folder. Confirm merging
		the folders.</li>
		<li>Start Code::Blocks.</li>
		<li>Click <tt>Create new project...</tt>.</li>
		<li>Select <tt>Dynamic Link Library</tt> and click \c Go.</li>
		<li>Click \c Next.</li>
		<li>Enter the name of your plugin (here: \c are_you_sure). To avoid
		problems later, do not use spaces.</li>
		<li>In the second field, select the \c DynPlugins folder of your
		RM2k3 test project. The actual filename of the Code::Blocks project
		file (shown in the last field) should be similar to this:
		<tt>C:\\Users\\</tt><i><tt>My Username</tt></i><tt>\\Documents\\</tt><i><tt>My Test Project</tt></i><tt>\\DynPlugins\\are_you_sure\\are_you_sure.cbp</tt></li>
		<li>Click \c Next.</li>
		<li>Make sure <tt>GNU GCC Compiler</tt> is selected in the first field.</li>
		<li>Uncheck <tt>Create "Debug" configuration</tt>.
		\note Of course, if you know how you use them, you may create debug builds
		too. As said, these step-by-step instructions are for beginners.</li>
		<li>Change the <tt>Output dir.</tt> at the <tt>"Release" options</tt> to
		<tt>..\\</tt> (two dots and then a backslash). This will create the plugin
		DLL directly in the \c DynRPG folder if you have followed step 11
		correctly.</li>
		<li>Click \c Finish.</li>
		<li>Open the \c Project menu, select <tt>Build options...</tt>.</li>
		<li>Go to the <tt>Linker settings</tt> tab.</li>
		<li>At the <tt>Link libraries</tt> section, click \c Add.</li>
		<li>Enter <tt>DynRPG</tt> and click \c OK.</li>
		<li>Go to the pre/post build steps tab.</li>
		<li>Enter the following line into the bottom box: <tt>cmd /c del ..\*.a & del ..\*.def</tt></li>
		<li>Click \c OK again.</li>
		<li>Open the \c Project menu, select <tt>Set programs' arguments...</tt>.</li>
		<li>Click the button at the right end of the <tt>Host application</tt> field
		and select your game's \c RPG_RT.exe file. This will allow you to use the
		\c Run and <tt>Build and run</tt> buttons in Code::Blocks to run your game.</li>
		<li>If you want the game to automatically start in windowed test play mode
		when you start it from Code::Blocks, put <tt>TestPlay ShowTitle Window</tt> into
		the <tt>Program arguments</tt> field.</li>
		<li>Click \c OK.</li>
		<li>Now, Code::Blocks has created two files for you: \c main.cpp (find it in
		the \c Sources folder of the tree view) and \c main.h (find it in the \c Headers
		folder of the tree view). Delete the contents of both of them. In our examples,
		we don't need the \c main.h file at all, you may right-click it and choose
		<tt>Remove file from project</tt> if you want. In \c main.cpp, copy the
		code provided below:</li></ol>

	\code
#include <DynRPG/DynRPG.h>

// Handler called on startup
bool onStartup(char *pluginName) {
	if(MessageBox(
		NULL, // We don't need a window handle
		"This is such a haaaaard game. Are you SURE you want to play it now?", // Text
		"The Are You Sure Plugin", // Title
		MB_YESNO | MB_ICONQUESTION // Flags (yes/no buttons and question icon)
	) == IDYES) {
		// The user clicked "Yes", we may continue
		return true;
	} else {
		// The user clicked "No", so we need to abort
		return false;
	}
}
	\endcode
	Okay, that's really simple. It uses Windows' <a href="http://msdn.microsoft.com/en-us/library/windows/desktop/ms645505%28v=vs.85%29.aspx">MessageBox</a>
	function to display a message. If the user clicked "Yes", the game will
	continue, otherwise it will not start.

	This simple plugin uses the \ref onStartup callback. This callback is
	called before the game starts. If you return \c false in this callback,
	the game won't start. (You only need to define those callbacks which you
	want to use.)

	So, if you compile this plugin, make sure the DLL file (\c are_you_sure.dll)
	was put into the \c DynPlugins directory and your test project was
	successfully patched using \ref patch, you should see our message box when
	you start the game.

	\section sdk_condition_icons Condition Icons - More Advanced
	Enough of nonsense. Now we are going to create something useful. We are
	going to create a plugin which displays the conditions of actors and
	monsters over their head, as icons.

	These are its features:
		<ul><li>It will read the filenames of the icons from the configuration
		file.</li>
		<li>It will load the files on demand (when they are needed the first
		time).</li>
		<li>It will display them over the head of the battlers, horizontally
		centered, vertically aligned at the bottom (in case the images don't
		have the same height).</li>
		<li>It will disable the default info window which is shown on target
		selection so that we don't show the conditions twice.</li></ul>

	First, in the \ref onStartup handler, we will load the configuration and
	store it in a global variable called \c configuration.

	Then, in the \ref onBattlerDrawn handler, we will iterate through all
	conditions the battler has, check if the images we need are loaded, if not,
	load them. We will also calculate the total width of all icons for that
	battler here. Then we will draw all the icons and finally clear the
	RPG::Battler::displayedConditions array so that no info window is displayed
	when the target selection is active.

	Finally, in the \ref onExit handler, we will unload all images.

	This is the content of our \c condition_icons.cpp file:
	\code
#include <DynRPG/DynRPG.h>
#include <sstream> // For std::stringstream

// We store the configuration here
std::map<std::string, std::string> configuration;

// We store the images of the conditions here
std::map<int, RPG::Image *> images;

// This handler is called on startup
bool onStartup(char *pluginName) {
	// We load the configuration from the DynRPG.ini file here
	configuration = RPG::loadConfiguration(pluginName);
	return true; // Don't forget to return true so that the start of the game will continue!
}

// This handler is called after a battler is drawn
bool onBattlerDrawn(RPG::Battler *battler, bool isMonster, int id) {
	int totalWidth = 0; // We store the total width of the condition icons here

	// We loop through all the elements of the battler's "conditions" array here
	// Note that the "condition" array is one-based, thus we start at index 1
	for(int i = 1; i <= battler->conditions.size; i++) {
		// If the battler has the condition (see documentation)...
		if(battler->conditions[i] > 0) {
			// If the image isn't loaded yet...
			if(!images[i]) {
				// First, we create an RPG::Image object
				images[i] = RPG::Image::create();

				// Yes, we want to use the transparent color
				images[i]->useMaskColor = true;

				// Now, we put the key name for the configuration entry together
				// It should be "Condition12" for condition 12, for example
				std::stringstream keyName;
				keyName << "Condition" << i;

				// Now, we try to load the image. If loading the image fails,
				// nothing special will happen (because of the "false" at the
				// "showErrors" parameter), the image will just be empty.
				images[i]->loadFromFile(configuration[keyName.str()], false);
			}

			// We add the image's width to the total width
			totalWidth += images[i]->width;
		}
	}

	// Now we need to know the Y coordinate of the top of the battler
	int topY;
	if(isMonster) {
		// It is a monster. We just use the monster's image to find out
		// its size.
		RPG::Monster *monster = (RPG::Monster *)battler; // First, we cast the "battler" to a "Monster"
		topY = monster->y - monster->image->height / 2; // Now we calculate the top position
	} else {
		// Okay, since we don't have a way to find out the size of an actor's
		// battle graphic, we will just "guess" that it's a normal BattleCharSet
		// which is 48 pixels tall.
		// In a "good" plugin, there would be a way to set the actual height
		// for each actor in the configuration, but this would be too much
		// for this tutorial.
		topY = battler->y - 48 / 2;
	}

	// We will use this variable to store the current X coordinate while we
	// draw the images. We will increase this variable every time we draw an
	// image.
	int currentX = battler->x - totalWidth / 2;

	// Okay, let's loop again through the conditions. This is necessary
	// because we first had to find out the total width before we could
	// start drawing the images. Now we will draw them.
	for(int i = 1; i <= battler->conditions.size; i++) {
		// If the battler has the condition (see documentation)...
		if(battler->conditions[i] > 0) {
			// Okay, here we actually draw the image on the screen!
			RPG::screen->canvas->draw(currentX, topY - images[i]->height, images[i]);

			// And we increase the current X coordinate.
			currentX += images[i]->width;
		}
	}

	// Clear the condition cache which is normally used to determine which
	// conditions should be shown in the "info window" on top of the screen
	// during target selection (we don't need to display the conditions twice!)
	for(int i = 0; i < 5; i++) {
		battler->displayedConditions[i] = 0;
	}

	return true; // It's okay that the battler is drawn, so we return true. Don't forget that!
}

// This handler is called when the game exits
void onExit() {
	// We will unload all images here.
	// If you are not familiar with C++ iterators: This "for" loop just iterates
	// through all items in "images".
	for(std::map<int, RPG::Image *>::const_iterator iter = images.begin(); iter != images.end(); ++iter ) {
		// The reason we don't use images[iter->first] instead of iter->second
		// is that RPG::Image::destroy also takes a reference and sets the
		// parameter to zero at the end.
		RPG::Image::destroy(images[iter->first]);
	}
}
	\endcode

	To try it, we just need to put the compiled DLL into the \c DynPlugins
	folder (if it isn't already there). Then we need to get some icons. For
	testing, I made some very simple 16x16 icons and put them into the
	\c Picture folder.

	Then we need to open our DynRPG.ini and put the filenames in it, like this:
	\code
[condition_icons]
Condition2=Picture\poison.png
Condition3=Picture\blind.png
Condition5=Picture\berserk.png
Condition6=Picture\confused.png
Condition7=Picture\sleep.png
	\endcode

	And hey presto, this is what we get:

	<img src="files/screen_condition_icons.png"/>

	You can download the source code as well as the finished plugin <a href="http://rpg-maker.cherrytree.at/dynrpg/files/condition_icons.rar">here</a>.

	\section sdk_what_next What next?
	You may now start developing your own plugins. You may first take a look
	at the \ref callbacks, the \ref game_objects, and the members of the RPG
	namespace, to get an overview of what is possible and how to get to the
	result you want. Then, you might start with something easy (like a
	DynRPG-based version of the <a href="http://cherrytree.at/download/?did=19">PicPointerPatch</a> -
	hint: take a look at \ref onEventCommand, RPG::EventScriptLine and
	\ref RPG::variables), then expand your knowledge until you can do nearly
	everything you want... or something like that. :-)

	<b>Yes, I know, I am just throwing you out into the wilderness. Sorry for
	not providing you more examples how to use all the features. I just had
	no time for it yet. If you are clever, you will probably be able to create
	something useful with what you got in this documentation anyway. If not,
	more examples will come. I will create nice plugins and publish them with
	source code, etc.</b>

	Oh, and don't forget to read and follow the \ref guidelines!

	By the way: If you want to publish your plugin, it would be nice if you
	would publish the source code too.

	Have fun!\n
	Best regards, Cherry
*/
