package airhockeyjava.game;

/**
 * 
 * Top-level class for constants. This allows us to cleanly and statically reference constants
 * throughout the project code, and make necessary changes quickly.
 * 
 * @author Joshua Segeren
 *
 */
public class Constants {
	public static final float GAME_TIME_SECONDS = 5f;
	public static final float GAME_TABLE_HEIGHT_METERS = 1.8f;
	public static final float GAME_TABLE_WIDTH_METERS = 3f;
	public static final float GAME_PUCK_RADIUS_METERS = 0.025f; // 2.5 centimetres
	public static final float GAME_MALLET_RADIUS_METERS = 0.035f; // 3.5 centimetres

	// Quick visual of coordinate convention being used here, where tuples are in (x,y) form:
	//
	// (0,0) <---- x ----> (WIDTH, 0)
	//	   _____________
	//     |            |
	//  y  |            |
	//     |____________|
	// (0, HEIGHT)         (WIDTH, HEIGHT)
	// 
	public static final float GAME_PUCK_INITIAL_POSITION_X = GAME_TABLE_WIDTH_METERS / 2f;
	public static final float GAME_PUCK_INITIAL_POSITION_Y = GAME_TABLE_HEIGHT_METERS / 2f;
	public static final float GAME_PUCK_INITIAL_VELOCITY_X = 0f;
	public static final float GAME_PUCK_INITIAL_VELOCITY_Y = 0f;
	public static final float USER_MALLET_INITIAL_POSITION_X = GAME_TABLE_WIDTH_METERS;
	public static final float USER_MALLET_INITIAL_POSITION_Y = GAME_TABLE_HEIGHT_METERS / 2f;
	public static final float USER_MALLET_INITIAL_VELOCITY_X = 0f;
	public static final float USER_MALLET_INITIAL_VELOCITY_Y = 0f;
	public static final float ROBOT_MALLET_INTIIAL_POSITION_X = 0f;
	public static final float ROBOT_MALLET_INITIAL_POSITION_Y = GAME_TABLE_HEIGHT_METERS / 2f;
	public static final float ROBOT_MALLET_INITIAL_VELOCITY_X = 0f;
	public static final float ROBOT_MALLET_INITIAL_VELOCITY_Y = 0f;
}