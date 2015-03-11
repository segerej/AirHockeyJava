package airhockeyjava.strategy;

import airhockeyjava.game.Constants;
import airhockeyjava.game.Game;
import airhockeyjava.util.Vector2;

/**
 * Strategy layer. Naive implementation which considers all feasible combinations of moves, and chooses
 * move most likely to result in a goal.
 * NAIVE DEFENSE simply follows the side-to-side path of the puck.
 * 
 * @author Joshua Segeren
 *
 */
public class NaiveDefenseStrategy implements IStrategy {

	final private static String strategyLabelString = Constants.STRATEGY_NAIVE_DEFENSE_STRING;
	final private Game game;

	public NaiveDefenseStrategy(Game game) {
		this.game = game;
	}

	@Override
	public Vector2 getTargetPosition(float deltaTime) {
		Vector2 nextPostion;
		
		Vector2 puckPosition = game.gamePuck.getPosition();
		if(puckPosition.x < game.gameTable.getWidth() / 2){
			return new Vector2(Constants.ROBOT_MALLET_INTIIAL_POSITION_X, Constants.ROBOT_MALLET_INITIAL_POSITION_Y);
		} else {
			nextPostion = new Vector2(Constants.ROBOT_MALLET_INTIIAL_POSITION_X, game.gamePuck.getPosition().y);
			float positionDiff = Math.abs(nextPostion.dst(game.robotMallet.getPosition()));
			return positionDiff > Constants.STRATEGY_MOVEMENT_TOLERANCE ? nextPostion : game.robotMallet.getPosition();
		}
		
	
	}

	@Override
	public String toString() {
		return strategyLabelString;
	}

	@Override
	public void initStrategy() {
		// TODO Auto-generated method stub
		
	}
}