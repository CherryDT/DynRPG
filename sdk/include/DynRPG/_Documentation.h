/*!
	\mainpage About DynRPG

	\author David "Cherry" Trapp

	<i><b>Also visit the main page, <a href="http://cherrytree.at">Cherry
	Tree</a>!</b></i>

	\section download Download
	Yes, this is the first paragraph, because I know people are looking for it
	most of the time, probably. So be it.

	<b><a href="http://rpg-maker.cherrytree.at/dynrpg/files/dynrpg.rar">Click me hard!</a></b>

	\section contact Contact
	And yes, this is the second paragraph, because people seek this information
	very often too.

	If you have any questions, you may either ask in of the following forum
	threads:
		<ul><li><b>English:</b> <a href="http://rpgmaker.net/forums/topics/10254/">Thread on RMN</a></li>
		<li><b>German:</b> <a href="http://www.multimediaxis.de/threads/134256">Thread on Multimediaxis</a></li>
		<li><b>French:</b> <a href="http://www.rpg-maker.fr/index.php?page=forum&id=19911">Thread on Oniromancie</a> (my reponses will be in English though)</ul>
	...or write me an e-mail at <a href="mailto:dynrpg@cherrytree.at">dynrpg@cherrytree.at</a>!

	Oh, and if you find errors in this documentation, or bugs in the patch or
	the SDK, please tell me too!

	\section introduction Introduction
	DynRPG is a plugin SDK for %RPG Maker 2003. "SDK" means "Standard Development
	Kit", and in this case it means that <b>everyone can create his own
	extensions to the %RPG Maker software now</b>, as long as he knows how to use
	C++. DynRPG not only offers the average programmer access to the world of
	"patching", but it also provides the basis for far more powerful extensions
	than all "patches" made for the %RPG Maker before.

	DynRPG works using plugins. Plugins can be written in C++, they will be
	loaded when the game starts and they are notified of certain events (like
	the drawing of a new frame, the writing to a variable, etc.) and are
	allowed to react to these events.

	What in past were "patches" are now going to be simple plugins. Any game
	using DynRPG can be extended by a new feature by simply putting a plugin
	file into a folder called \c DynPlugins. That's far easier than "patching",
	something which many game makers are even afraid of, because they think it
	might harm their game. And if somebody doesn't like a certain plugin, all
	he needs to do is deleting it.

	So, DynRPG is beneficial to both <b>plugin makers</b> and <b>game
	makers</b>:
		<ul><li>Plugin makers can use a simple C++ library to write powerful
		plugins and distribute them to other game makers!</li>
		<li>Game makers get many new possibilities just by adding these plugins
		to their game!</li>
		<li>Professional game makers are able to write plugins which exactly fit
		to their game's needs, for example they might write part of their action
		battle system in C++ or apply a custom design to the default battle
		system, or maybe they even create a whole minigame using DynRPG... the
		possibilities are endless!</li></ul>

	\section important_links Important links
	<b>Are you a game maker?</b> Then look at \ref patch to find out how to
	install and use the DynRPG Patch and what features it has!

	<b>Are you a plugin maker?</b> Then you might start with the
	\ref getting_started section, and make sure you follow the \ref guidelines!
	Of course, you can also explore the SDK documentation using the navigation
	bar and the search function. The two main sections you need are the RPG
	namespace and the \ref callbacks.

	\section parts Parts
	The name "DynRPG" means this project as a whole... but actually, it consists
	of several parts:
		<ul><li><b>The DynRPG Patch:</b> This is the only real patch here. Yes,
		I was not completely honest: The game makers don't get <i>completely</i>
		rid of "patching", since they have to install this one patch. It is a
		small patch for %RPG Maker 2003 version 1.08, which loads the DynRPG
		Loader at startup, notifies it of game events and also applies several
		bugfixes to the %RPG Maker. The DynRPG Loader then looks for plugins,
		loads them and later forwards game events to them. The DynRPG Patch is
		compatible with most of the other %RPG Maker 2003 patches out
		there. More information here: \ref patch</li>
		<li><b>The DynRPG Patcher:</b> This simple tool installs the DynRPG
		Patch for you.</li>
		<li><b>The DynRPG Loader:</b> That's a little DLL file called
		\c dynloader.dll which is the middle-man between the DynRPG Patch
		and the plugins.</li>
		<li><b>The DynRPG SDK:</b> That's the library and the C++ header files
		which allow you to create your own plugins for DynRPG very easily. (If
		you are no programmer but just a game maker looking for ready-made
		plugins, you don't need to bother yourself with this.)</li>
		<li><b>The plugins:</b> Although not actually part of the DynRPG
		project, the plugins are what makes the whole thing useful. They can
		extend the features of the %RPG Maker (as in the past patches did), but
		in a far more powerful way. A plugin is a simple DLL file which is
		placed in the \c DynPlugins folder of a game.</ul>

	\section Credits
	I want to thank <b>Bananen-Joe</b> for helping me when I had questions -
	all the time since I got to know him... and also for his Destiny Patch
	because it gave me the competition I needed. :-)

	I also want to thank <b>MagicMaker</b> for testing all my stuff before.
	Although he wasn't able to test DynRPG itself, his contribution also
	helped me with my previous research... and that research is the base
	of DynRPG.

	And last but not least, I want to thank <b>Crash-Override</b> alias
	<b>WordsBG</b> for trying to "own me" years ago when I created my first
	"patch" (the RM2k(3) Font Changer)... I think if he hadn't challenged me,
	I wouldn't have dug into reverse engineering and I would never have come
	this far. (Explanation: After I published the font changer tool, he
	created a thingy called "RM Binary Patcher" which basically did the same
	as my font changer and Miroku's "Maker Ultimative" patcher combined. The
	latter had several bugs, and because he commented his tool with "because
	I just wanted to own you", I took that as a challenge and created a better
	tool, <a href="http://cherrytree.at/download/?did=9">Hyper Patcher 1</a>,
	which did the same as his tool, but without bugs and with more features.
	He then admitted that I had won.)

	\page patch The Patch
	\section installation_patch Installation
	It couldn't be easier: You just run the patcher (that's the file called
	\c dynrpg_patcher.exe), select your project's \c RPG_RT.exe file and that's
	it, at least in most of the cases. DynRPG should be compatible with all
	other 1.08-based patches.

	In case you don't have \c RPG_RT.exe version 1.08, the patcher will	offer
	you to install it. However, this will remove all existing modifications
	(other patches, custom icon, etc.). But don't worry, in case anything goes
	wrong there is always a backup (that's the file with the <tt>.bak</tt>
	extension). If you install the patch "over" an existing (older) version of
	the DynRPG Patch, there will also be a backup of the \c dynloader.dll
	file.

	You don't need to include the backup files (ending with <tt>.bak</tt>) in
	your finished game. Also, players don't need to install anything special to
	run a game which uses DynRPG.

	The patcher will also automatically create a \c DynPlugins folder for you,
	so you can immediately start adding and using plugins.

	\section features_patch Extra Features
	Of course, the main feature of the DynRPG Patch is interfacing the DynRPG
	Loader, which then loads the plugins. (See \ref parts for more information.)
	\note You cannot use more than 50 plugins at the same time.

	But that's not everything, the patch also comes with several <b>improvements
	and bugfixes</b> for the %RPG Maker:
		<ul><li><b>Improvement: <i>More pictures!</i></b> You can now use 2000 pictures instead of
		50, and pictures \#1001 to \#2000 are not erased when the player goes
		to a new map! Please read the \ref more_pics section below to find
		out what you need to know about this feature.</li>
		<li><b>Improvement: <i>On-the-fly application of IPS patches!</i></b>
		This means that IPS patches can now be used in a similar manner as plugins,
		by simply copying them to a folder. This means they can easily be removed
		again by deleting the file. Please read the \ref onthefly_ips section
		for further details.</li>
		<li><b>Improvement: <i>Quickpatches!</i></b> This feature allows you to
		add simple patches as small lines of text which can easily be shared on
		the Internet. Those simple patches can also be customized (such as having
		a configurable value for something). Please read the \ref quickpatches
		section below for detailed instructions!</li>
		<li><b>Improvement:</b> Monsters are now flashed more intensively
		when they do their turn. Before, it was hard to follow what was going on
		in battle, especially if a monster used the "Attack" command.</li>
		<li><b>Improvement:</b> Picture operations now also work while a message
		is displayed. (Yes, that's the same thing the
		<a href="http://cherrytree.at/download/?did=16">UnlockPics Patch</a>
		does.)</li>
		<li><b>Improvement:</b> The game doesn't completely close when an error
		occurs. Instead it goes back to the title screen.</li>
		<li><b>Improvement:</b> You can now press \c F11 to go to the save
		menu at any time (in test play only).</li>
		<li><b>Improvement:</b> The "Input Number" event command will now use
		the existing value in the variable as initial value. Before, it always
		started with zero.</li>
		<li><b>Improvement:</b> When you press \c ESC at a "Show Choice"
		command where the cancel behavior is set to "Ignore", now the buzzer
		sound is played.</li>
		<li><b>Improvement:</b> Transparent message windows will now also work
		with the "Traditional" battle layout (at least out of the battle). You
		have to change the battle layout so that the "Transparent" checkbox
		becomes enabled, check or uncheck it and then change the battle layout
		back.</li>
		<li><b>Bugfix:</b> The game would crash when you removed an actor in
		battle when this actor was the last to do his turn.</li>
		<li><b>Bugfix:</b> The "Enemy Attack" sound was never played.
		<li><b>Bugfix:</b> The game would crash if a skill was reflected on an
		actor with an ID higher than the number of actors in the party.</li>
		<li><b>Bugfix:</b> The battle status window was gone while the skill
		selection window was open.</li>
		<li><b>Bugfix:</b> If you had selected "Small Window" in the "Battle
		Layout" tab of the database, the action selection window would only
		display 3 lines plus an ugly empty one at the bottom and also the item
		and skill selection windows would not display the last line correctly.
		Additionally, the selection cursor for "Show choice" windows was
		misaligned in battle.</li>
		<li><b>Bugfix:</b> The "Order" menu (used for rearranging party members)
		didn't play the cancel sound when you exited.</li>
		<li><b>Bugfix:</b> When the shop system was used, the events weren't
		updated, which means that if you bought an item in the shop, events
		which used this item as start condition wouldn't execute
		immediately.</li>
		<li><b>Bugfix:</b> HP display in the save menu was misaligned when the
		HP had 4 digits.</li>
		<li><b>Bugfix:</b> The game would crash if the player would open the
		item window in battle, then close it again using ESC and then execute
		a battle command of type "Link to Event".</li>
		<li><b>Bugfix:</b> The game would crash with a "Division by zero"
		error on certain edge cases of picture operations (especially pictures
		with small zoom level partly outside the screen).</li>
		<li><b>Bugfix:</b> Enemies were flashed even when they executed the
		"Do nothing" action.</li></ul>

	\section more_pics About the pictures
	The DynRPG patch increases the picture limit to 2000. You might have heard
	that <a href="http://cherrytree.at/download/?did=10">Hyper Patcher 2</a> is
	able to increase this limit to a crazy 9999, but this is not so healthy in
	this case. Using that many pictures takes several seconds to load on
	starting or loading a game or going to title screen, plus you would have
	very bad performance with DynRPG because the DynRPG SDK allows plugins to
	intercept picture drawing. Each plugin is called once before and once after
	the picture is to draw, so you can imagine that 9999 pictures would mean 5
	times more plugin calls than 2000 pictures, so I decided that 2000 pictures
	would be the best compromise.

	Please keep in mind that <b>only pictures \#1 to \#1000 will be erased on
	map change</b>! The fact that pictures with an ID greater than 1000 won't be
	erased automatically allows you to use pictures for cross-map purposes more
	easily.

	However, there is one thing you need to do (unless you are already using
	<a href="http://cherrytree.at/download/?did=19">PicPointerPatch</a>): You
	need to modify the %RPG Maker editor itself too, otherwise you won't be able
	to select picture IDs greater than 50 in the event editor. There are two
	ways to do this:
		<ul><li><b>The easy way:</b> You need to use
		<a href="http://cherrytree.at/ultimate">%RPG Maker 2009 Ultimate</a>.
		You then just need to create a file called \c morepictures.ini (or
		whatever name you like) in %RPG Maker 2009 Ultimate's \c uimod folder
		and put the following text into it:
		\code
[FormEvCmd11110]
DialEdit1.MaxValue=100000

[FormEvCmd11120]
DialEdit1.MaxValue=100000

[FormEvCmd11130]
DialEdit1.MaxValue=100000
		\endcode
		Then you need to edit the \c ultimate.ini file: Open it in a text
		editor, go to the section <tt>[UIMod]</tt> and add your file at the end
		of the section.</li>
		<li><b>The hard way:</b> If you do not want to use %RPG Maker 2009
		Ultimate, you need to directly edit your \c RPG2003.exe file:
			<ol><li>Download
			<a href="http://www.snapfiles.com/get/resourcehacker.html">Resource
			Hacker</a>.</li>
			<li>Open your \c RPG2003.exe file in it.</li>
			<li>Navigate to <tt>RCDATA \> TFORMEVCMD11110 \> 0</tt>.</li>
			<li>In the script on the right side, look for the following line:
			\code
MaxValue = 50
			\endcode
			If you have an older %RPG Maker version, it might be \c 40
			instead of \c 50. Increase this value, for example, to
			\c 100000. The reason we use such a high value is that higher
			values might be used for extra features (for example, with the
			<a href="http://cherrytree.at/download?did=19">PicPointerPatch</a>).</li>
			<li>Click <tt>Compile Script</tt>.</li>
			<li>Repeat steps 4 and 5 for the pages <tt>RCDATA \> TFORMEVCMD11120
			\> 0</tt> and <tt>RCDATA \> TFORMEVCMD11130 \> 0</tt>.
			<li>Save your changes and exit Resource Hacker.</li></ul></ul>
	
	\section onthefly_ips On-the-fly IPS patches
	DynRPG can parse IPS files and apply them to the game in memory. This means
	you can add IPS patches without actually modifying any file on your harddisk,
	making it easy to remove the patch again. There is a folder \c DynPatches
	in which the loader will look for IPS files and apply them on-the-fly. Please
	note that the loader will fail to apply a patch if it tries to modify regions
	of the \c RPG_RT.exe which would affect DynRPG itself or lie outside of the
	areas which are mapped to memory. (Don't worry, nothing will explode, it will
	just trigger an error message.)
	
	\section quickpatches Quickpatches
	Quickpatches are simple text "codes" which instruct DynRPG to modify certain
	bytes in the game's memory. They are added to a special section in your
	\c DynRPG.ini file called <tt>[QuickPatches]</tt>. Each quickpatch has a name,
	which can be arbitrary and is completely ignored by the loader.
	
	<b>End users:</b> Simply copy and paste a quickpatch from the Internet into
	your <tt>[QuickPatches]</tt> section (create the section if it does not exist).
	If you don't like it, remove it again or disable it by prepending a
	semicolon. Quickpatches may contain configurable values, the developer
	should normally tell you what their effect is.
	
	<b>Developers:</b> The format of quickpatches is very simple:
	\code
PatchName=Address1,Values1,Address2,Values2,Address3,Values3,...
	\endcode
	You can specify one of more virtual address to modify, together with the
	values to write to that address. These "values" may use one of those 3
	formats:
	<ul><li><b>Hex bytes:</b> One or more bytes in simple two-digit hexadecimal
	notation. For example, <tt>401234,90</tt> will write byte \c 0x90 at address
	\c 0x401234, <tt>401234,1A2B3C</tt> will write byte \c 0x1A at address
	\c 0x401234, byte \c 0x2B at address \c 0x401235 and byte \c 0x3C at address
	\c 0x401236.</li>
	<li><b>8-bit decimal value:</b> Decimal value (may be negative) prepended by
	a percent sign, which will occupy one byte. Meant to be easily customizable
	by end users. For example, <tt>401234,%32</tt> will write byte \c 0x20 at
	address \c 0x401234.</li>
	<li><b>32-bit decimal value:</b> Decimal value (may be negative) prepended by
	a sharp sign, which will occupy four bytes. Meant to be easily customizable
	by end users. For example, <tt>401234,#1000</tt> will write byte \c 0xE8 at
	address \c 0x401234, byte \c 0x03 at address \c 0x401235, and two \c 0x00 bytes
	at addresses \c 0x401236 and \c 0x401237.</li></ul>
	
	Example for two "real-life" quickpatches:
	\code
; Hides EXP in main menu and save screen
HideEXP=49E148,EB71,49F1CA,EB67,49F095,EB21
; Moves the window on the title screen to coordinates 144/56
MoveTitleWindow=490821,#144,490828,#56
	\endcode
	For this example please note that the %RPG Maker uses the X coordinate for the
	middle of the window, while the Y coordinate is used for the top of the window.
	
	\page guidelines Rules and guidelines for plugin developers
	\section rules Rules
	The %RPG Maker is written in Delphi (and I didn't have access to the source
	code of its classes, etc.), while my SDK uses C++. Thus, many things are
	not working the way you might expect.

	There is a set of rules which you must follow under all circumstances when
	you are developing a DynRPG Plugin:
		<ul><li><b>Do not try to use members which are not documented.</b> They
		are either unknown or used internally and dangerous. (Of course, if you
		<i>know</i> what a member does, it's a different story.)</li>
		<li><b>Do not instantiate RPG classes.</b> Always use pointers to
		existing instances you get from DynRPG. It is especially dangerous to
		use these "home-made" objects with functions from the RPG namespace. It
		might seem to work, but will most likely corrupt data. This will cause
		the game to behave strangely or suddenly crash some minutes later.
		<b>An exception to this rule are the RPG::Music and RPG::Sound
		classes.</b></li>
		<li><b>Do not assign RPG objects.</b> The result is mostly undefined.
		An exception are pointers to RPG::Image objects, as long as you don't
		forget to destroy the old object using RPG::Image::destroy (unless you
		want to use it before).</li>
		<li><b>Do not randomly return \c false from a callback
		function.</b> This will "lock out" all plugins which are called after
		yours. Only return \c false if you really want this behavior.</li>
		<li><b>Never change the \c vTable member of a class.</b> This will
		make the game crash sooner or later (probably sooner).</li>
		<li><b>Never assign a <tt>char *</tt> to an RPG::DString pointer.</b>
		It will appear to work, but it will cause the game to crash with an
		"Invalid pointer operation" error when the %RPG Maker tries to free the
		string. Also, do not store RPG::DStringPtr objects or RPG::DString
		pointers inside your plugin, but copy their content to a
		\c std::string instead, since RPG::DString objects may suddenly
		vanish.</li>
		<li><b>Do not change the current directory.</b></li>
		<li><b>Do not do stuff every frame which takes longer than one
		millisecond.</b> This is alredy the very maximum. The less time
		you use, the better. If you need to do something which takes
		longer, do it in another thread. An exception are things which
		happen only rarely, like when a game is loaded or saved, or once
		when a battle starts, etc. If something should intentionally take
		longer than one frame, you could use the \ref RPG::Screen::update or
		RPG::updateBattle function, respectively.</ul>

	\section guidelines_sec Guidelines
	There is also a set of guidelines which you are strongly advised to follow,
	but there might be cases in which there is a better solution.

	\subsection event_comments Event comments
	Comments in event scripts are a great way to let events scripts invoke
	functions of your plugin. Please follow the following guidelines:
		<ul><li>Use the following pattern for "special comments":
			\code
@command parameter1, parameter2, parameter3, ...
			\endcode
		</li>
		<li>New-line characters should be generally ignored.</li>
		<li>The comment has to start with an <tt>\@</tt> sign, immediately
		followed by the command name.</li>
		<li>The command name is case-insensitive.</li>
		<li>There should be three possible types of parameters:
			<ul><li><b>Number:</b> A simple number. Can also use the decimal
			point and the scientific notation (e.g. <tt>5.5e+6</tt> for 5.5
			million).</li>
			<li><b>String:</b> A simple string. Must be put between
				doublequotes. To use a doublequote in a string, it is
				written twice (e.g. <tt>He said ""hello"" and smiled</tt>)</li>
			<li><b>Token:</b> Some identifier. Tokens are not put between
			quotes, and they may not contain spaces (spaces are removed).
			Tokens are case-insensitive. They may be used for keywords.</li></ul>
			There are special tokens for referencing variables and actor
			names:
			<ul><li><b>Variables:</b> To reference a variable, the user should
			be able to write a \c V character prior to the variable ID. This
			should also work with multiple levels of dereferencing.</li>
			<li><b>Actor names:</b> To reference an actor's name, the user
			should be able to write a \c N character prior the the actor ID.
			This should also work together with \c V.</li></ul>
			<li>The command name and tokens should be case-insensitive.</li>
			<li>Always return \c false from \ref onComment when you found
			a known command, regardless whether the parameters were valid or
			not.</li>
			<li>Always return \c true from \ref onComment when you didn't
			find a known command, even though you may have found an <tt>\@</tt>
			sign at the beginning of the comment.</li></ul>

		<b>Use the \c parsedData parameter of your \ref onComment handler to get
		the comment data in an already nicely parsed form!</b>
		\note The maximum number of parameters is 100. The maximum number of
		characters per parameter (or command name) is 200. You have to parse the
		comment yourself if you need more.

		Example for a "special" comment:
		\code
@FooBar 123, "abc", V55, VV66, N7, NV8, Nothing
		\endcode
		The command name is \c foobar.\n
		The first parameter is numerical.\n
		The	second parameter is a string.\n
		The third parameter is a numerical value read from variable \#55.\n
		The fourth parameter is a numerical value read from the variable
		whose ID is stored in variable \#66.\n
		The fifth parameter is a string, read from the name of actor \#7.\n
		The sixth parameter is a string, read from the name of the actor
		whose ID is stored in variable \#8.\n
		The seventh parameter is a token named \c nothing.

		You might advise users to download
		<a href="http://cherrytree.at/ultimate">RPG Maker 2009 Ultimate</a>
		if they need to enter comments longer than 4 lines.

		\subsection configuration Configuration
		Many plugins need some kind of configuration. An important rule is:
		<b>Make as many things configurable as possible.</b>

		If possible, store configuration in a \c DynRPG.ini file. Also,
		you should use your plugin's name which you get as parameter to the
		\ref onStartup function as section name. If you need several sections,
		you can append an underscore and an additional identifier to the name
		and use it as section name. This will prevent conflicts with other
		plugins while still combining all relevant configuration of a game in
		one file.

		You may use the RPG::loadConfiguration function as a convenient way to
		load configuration data to a <tt>std::map\<std::string,
		std::string\></tt> in the \ref onStartup function.

		If you need more or more complex configuration, like XML data, use a
		filename containing your plugin's name.

		\subsection ingame_data In-game data
		Your plugin may also use data which is changed in-game and needs to be
		preserved. Savestate-independent data (like a highscore) should be
		stored in the \c DynRPG.ini file together with configuration
		(use the WinAPI function
		<a href="http://msdn.microsoft.com/en-us/library/windows/desktop/ms725501(v=vs.85).aspx">WritePrivateProfileString</a>).

		Savestate-related data (data which should be saved when the user saves
		the game and loaded when the use loads a saved game) should be saved
		using the function passed as \c savePluginData parameter to the
		\ref onSaveGame function. When the user loads the savestate again, you
		will get the same data back, in the parameters to the \ref onLoadGame
		function. Internally, this data is saved in a file called
		\c SaveXX.dyn where \c XX is the savestate ID.

		An example usage of savestate-related plugin data is shown here:
		\code
// Plugin-related data
int score;
int level;

// ...

void onLoadGame(int id, char *data, int length) {
	if(length == sizeof(int) * 2) { // make sure it is valid data
		int *dataArray = (int *)data;
		score = dataArray[0];
		level = dataArray[1];
	}
}

void onSaveGame(int id, void __cdecl (*savePluginData)(char *data, int length)) {
	int[2] dataArray;
	dataArray[0] = score;
	dataArray[1] = level;
	savePluginData((char *)dataArray, sizeof(dataArray));
}
		\endcode
		(Of course, the same result could have been achieved by saving \c score
		and \c level in in-game variables which are automatically saved.)

		\subsection optimization Optimization
		<b>Time is a very important factor.</b> Especially with many plugins,
		it is important to use as little time as possible in your callback
		handlers, otherwise the game will eventually start lagging. Thus, try to
		optimize your code where you can. If possible, always test your plugin
		with several other plugins in a "real-life situation" to see whether
		your plugin slows the game down too much. Remember that most of your
		code will be executed a minimum of 60 times per second (many parts more
		often than that, for example \ref onCheckEventVisibility will be called
		900 times per second if there are 150 events on the map).

		Here is a bit of advice how to optimize your plugin code:
			<ul><li><b>Do not allocate and deallocate memory or objects over and
			over again.</b> Try to use static variables wherever
			possible.</li>
			<li>Try to use functions in the RPG::Image and RPG::Canvas classes
			as little as possible. The slowest functions are
			RPG::Image::drawText and RPG::Canvas::draw.
			<li>Try to cache text in an RPG::Image if possible, only
			update it if necessary.</li>
			<li>Try to cache as much with the same palette as possible on
			the same RPG::Image so that you don't have to call
			RPG::Canvas::draw too often.</li>
			<li>If possible, don't use a transparent color in
			RPG::Image::draw (set \c maskColor to
			RPG::MASK_NONE). The same rule applies for RPG::Canvas::draw,
			see also RPG::Image::useMaskColor.</li>
			<li>Try to skip frames if possible. This means: Try to update some
			things only every 2 or 3 frames if possible.</li>
			<li>If you are "WinAPI-literate", you can use the
			RPG::Canvas::bitmap member and the
			RPG::DBitmap::getHBITMAP and RPG::DBitmap::getHDC functions to get
			handles to the corresponding GDI objects and manipulate them
			directly.</li>
			<li>If you need to calculate something more complex (like a 3D
			image, etc.) you better do this in a new thread, cache its graphics
			and draw them to the screen only after calculation was finished,
			without letting the main thread wait.</li></ul>

	\page changelog Changelog
	\section v0_20 Version 0.20 (2013/12/31)
		<ul><li>On-the-fly patching using IPS files or quick patches
		in DynRPG.ini added. See the corresponding \ref onthefly_ips and
		\ref quickpatches sections.</li>
		<li>Partial Hyper Patcher 2 picture limit changing support removed,
		it never worked correctly in the first place.</li>
		<li>\c AUTO_DLLMAIN define removed, now a \c DllMain function is always
		inserted by default, use <tt>\#define CUSTOM_DLLMAIN</tt> to prevent
		this behaviour.</li>
		<li>The automatic \c DllMain function now stores the plugin's instance
		handle in the global variable \c hInstance (only if \c CUSTOM_DLLMAIN
		is not defined).</li>
		<li><b>Critical bug fixed:</b> Game could hang when loading a game after
		a new plugin has been added due to an infinite loop bug during \c Save??.dyn
		file parsing.</li>
		<li><b>Critical bug fixed:</b> Transparency of events (including the hero)
		behaved weirdly (changed depending on Y position) and was thus unusable.
		This bug also slowed down the event rendering a lot.</li>
		<li><b>Bug fixed:</b> Class RPG::DList had members \c count and \c items
		swapped. Because DynRPG classes need to have the same memory layout as the
		%RPG Maker's internal Delphi counterparts, all accesses of RPG::DList
		classes (most notably RPG::monsters) crashed the game.</li>
		<li><b>Bug fixed:</b> RPG::Catalog::count didn't work (tried to access
		\c list.count where it should have been \c list.list->count, causing a
		compiler error.)</li>
		<li><b>Bug fixed:</b> RPG::Image::copy was broken (didn't set the new image
		size but silently leaked an RPG::Image instance instead).</li>
		<li><b>Bug fixed:</b> RPG::Actor and RPG::System methods sometimes returned
		wrong strings (i.e. for RPG::Actor::getName) if they encountered a default
		value in the database.</li>
		<li><b>Bug fixed:</b> Due to missing string termination, the
		RPG::ParsedCommentParameter::text member of a comment command's last
		parameter often had garbage appended at the end.</li>
		<li><b>Bug fixed:</b> Negative numbers in comment command parameters were
		incorrectly parsed as RPG::PARAM_TOKEN instead of RPG::PARAM_NUMBER.</li>
		<li><b>Bug fixed:</b> RPG::Event::doesEventPageExist was broken.</li>
		<li><b>Bug fixed:</b> RPG::transparentWindowsEverywhere was
		broken.</li>
		<li>Modified library to work with newer GCC compilers. Now GCC version
		4.7.1 is required. (For tech-guys: Why the hell can GCC now use the \c ESP
		register for an inline asm parameter with the \c "g" constraint?!)</li>
		<li><b>Bug fixed:</b> The "cross-map" pictures \#1001 to \#2000 were not
		erased when a new game was started.</li>
		<li>Fixed an %RPG Maker bug which caused the HP display in the save menu
		to be misaligned when the HP had 4 digits.</li>
		<li>Removed the longer skill/item window visibility "improvement" for most
		scenarious because it turned out to disrupt battle event processing.
		Only the info window shown when a monster executes a skill is still shown
		longer, but now only for 50 frames (old DynRPG used 90 frames which
		turned out to be annoyingly long, original %RPG Maker used 30 frames which
		I think is too short to be readable).</li>
		<li>Fixed an %RPG Maker bug which would cause the game to crash when
		a "Link to Event" battle command was used right after the item menu
		has been opened and then closed with ESC.</li>
		<li>Added RPG::Actor::twoWeapons, RPG::Actor::lockEquipment,
		RPG::Actor::autoBattle and RPG::Actor::mightyGuard fields.</li>
		<li>RPG::System::pedestrianBGM added. Funny name, I know.</li>
		<li>The maximum number of plugins has been raised from 30 to 50.</li>
		<li><b>Bug fixed:</b> Under some circumstances, the game would crash
		during event command execution. It's hard to describe the exact triggers,
		but they included "Erase event" commands in common events, loading a game
		which was saved while a "Wait until key press" was active and some other,
		rarer situations.</li>
		<li>Fixed two %RPG Maker bugs with "Small window" mode in battle: The
		action, item and skill selection windows had an empty line at the bottom
		even though there would have been enough space. This was especially
		confusing for the battle action window because it looked like the actor
		had only 3 commands available. Also, the selection cursor for "Show choice"
		windows in battle was not correctly aligned with the choice texts.</li>
		<li>Removed the "higher action window" workaround for the small battle
		window mode because a real fix has been implemented now.</li>
		<li><b>Bug fixed:</b> DynRPG would sometimes refuse to load a plugin
		with no valid reason, depending on the constellation of files in the
		DynPlugins directory.</li>
		<li>Fixed an %RPG Maker bug which would crash the game with a "Division
		by zero" error on certain edge cases of picture operations (especially
		pictures with small zoom level partially outside the screen). This bug also
		affected RPG::Canvas::drawCenteredZoomed.</li>
		<li><b>Bug fixed:</b> The enemy attack sound was also played if the enemy
		wasn't doing anything at all. Additionally, sometimes sleeping or paralyzed
		enemies would even flash white and the "Punch A" sound would play.</li>
		<li>Fixed an %RPG Maker bug which caused enemies to be flashed even when
		they executed the "Do nothing" action.</li>
		<li>The \ref onDoBattlerAction callback has now a second parameter
		\c firstTry which will be \c true only at the first attempt of executing
		the action. This allows plugins to execute code only once per action. Before,
		the \ref onDoBattlerAction callback could be called multiple times without
		a way to distinguish between a new action and a new attempt to execute
		the same action. Because the information about an action's success has
		to be stored somewhere, the RPG::Action::userData2 member has been
		repurposed and is now called RPG::Action::retrying. Old plugin DLLs which
		are unaware of the new \c firstTry parameter will still work, but if any
		old plugin tries to use the RPG::Action::userData2 member it will cause
		problems. However I've got the impression that it's unlikely that there
		is such a plugin out there.</li>
		<li>The member RPG::Action::userData1 has been removed from public access
		because I realized it's not a good idea to have generic fields without
		defined purpose, as this could easily cause conflicts between different
		plugins.</li></ul>

	\section v0_14a Version 0.14a (2012/02/15)
	Only the SDK was updated. The patch still shows version 0.13b.
		<ul><li>RPG::Character::mapID was renamed to RPG::Character::mapId to
		follow the naming convention used at all other places.</li>
		<li>The documentation now doesn't show inherited members anymore unless
		you click <tt>List of all members</tt>.</li>
		<li><b>Bug fixed:</b> RPG::Map::getWidth and RPG::Map::getHeight were
		returning nonsense due to a typo (again).</li></ul>

	\section v0_14 Version 0.14 (2012/02/15)
	Only the SDK was updated. The patch still shows version 0.13b.
		<ul><li>RPG::isBattleTest and RPG::showTitle added.</li>
		<li><b>Critical bug fixed:</b> The big bugfix done in version 0.13 created
		new bugs because I forgot to update a few things in the changed \c asm
		statements. Sorry for that.</li>
		<li><b>Bug fixed:</b> I had confused the parameters of RPG::Character::move.</li>
		<li><b>Bug fixed:</b> Nasty typo in RPG::encode caused it to produce garbage.</li>
		<li>Type of \c message parameter of RPG::showError changed from <tt>char *</tt>
		to <tt>std::string</tt>.</li>
		<li>Type of \c data parameter of RPG::Character::move changed from <tt>char *</tt>
		to <tt>const char*</tt>.</li></ul>

	\section v0_13b Version 0.13b (2012/02/15)
		<ul><li><b>Bug fixed:</b> If version 0.13a was patched over version 0.13,
		the game wouldn't start anymore. Patching 0.13b over 0.13a should fix the
		problem.</li></ul>
	\section v0_13a Version 0.13a (2012/02/13)
		<ul><li><b>Bug fixed:</b> Due to a silly typo the game crashed as soon
		as you would try to buy something in a shop.</li></ul>
	\section v0_13 Version 0.13 (2012/02/11)
		<ul><li><b>Critical bug fixed:</b> The \c asm statements in the SDK didn't
		properly tell the compiler that the \c eax, \c edx and \c ecx registers may
		be changed even though they often aren't used as output registers. This could
		randomly lead to faulty code (depending on the surrounding code and the
		compiler options) which would then result in strange crashes. <b>Please
		recompile all modules of your plugin with the new version of the header files
		and the library!</b></li>
		<li><b>Bug fixed:</b> The RPG::ParsedCommentData::command string wasn't
		terminated properly.</li>
		<li>I realized that Code::Blocks is far better than Dev-C++ and changed the
		\ref getting_started page accordingly.</li>
		<li>The RPG::showError function was added.</li></ul>

	\section v0_12 Version 0.12 (2012/02/07)
		<ul><li>RPG::Canvas::drawStretched and RPG::Canvas::drawCenteredZoomed added.</li>
		<li><b>Bug fixed:</b> The DynRPG Patch caused the battle status window to be always
		fully opaque in battle layout "alternative", even if "transparent" was
		enabled. If the latter was the case, additionally the ATB bar wasn't
		shown.</li>
		<li>Added "Powered by" on top of the DynRPG logo.</li></ul>

	\section v0_11 Version 0.11 (2012/02/06)
	Only the SDK was updated. The patch still shows version 0.10.
		<ul><li>RPG::BattleSettings class and RPG::battleSettings object added.</li></ul>

	\section v0_10 Version 0.10 (2012/02/05)
	This is the first release. I started this project on 2012/01/23, by the way. Just in case somebody wants to know.
*/
//! \defgroup game_objects Game objects
