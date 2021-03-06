package airhockeyjava.strategy;

import java.awt.geom.Line2D;
import java.awt.geom.Point2D;

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
	private Line2D[] defenceLines = new Line2D[1];

	public NaiveDefenseStrategy(Game game) {
		this.game = game;
		this.defenceLines[0] = new Line2D.Float(Constants.ROBOT_MALLET_INITIAL_POSITION_X,0f,Constants.ROBOT_MALLET_INITIAL_POSITION_X, Constants.GAME_TABLE_HEIGHT_METERS );
	}

	@Override
	public Vector2 getTargetPosition(float deltaTime) {
		if(game.guiLayer != null){
			game.guiLayer.strategyLines = defenceLines;
		}
		
		Vector2 nextPostion;
		
		Point2D collisionPoint = game.gamePuck.getExpectedInterectionPoint();
		
		Vector2 puckPosition = game.gamePuck.getPosition();
		if(puckPosition.x < game.gameTable.getWidth() / 3){
			return new Vector2(Constants.ROBOT_MALLET_INITIAL_POSITION_X, Constants.ROBOT_MALLET_INITIAL_POSITION_Y);
//		} else {
//			if(collisionPoint != null){
//				System.out.println("collision detected!");
//
//				return new Vector2((float)collisionPoint.getX(), (float)collisionPoint.getY());
//			}else{
//				System.out.println("following the puck:" + game.gamePuck.getPosition().y);
//				nextPostion = new Vector2(Constants.ROBOT_MALLET_INTIIAL_POSITION_X, game.gamePuck.getPosition().y);
//				float positionDiff = Math.abs(nextPostion.dst(game.robotMallet.getPosition()));
//				return positionDiff > Constants.STRATEGY_MOVEMENT_TOLERANCE ? nextPostion : game.robotMallet.getPosition();				
//			}
//
//		}
			
		} else {
			if(collisionPoint != null){
				System.out.println("collision point detected!");

				return new Vector2((float)collisionPoint.getX(), (float)collisionPoint.getY());
			}else{
				System.out.println("no collision point detected!");

				return new Vector2(Constants.ROBOT_MALLET_INITIAL_POSITION_X, Constants.ROBOT_MALLET_INITIAL_POSITION_Y);			
			}

		}
		
	
	}

	public String getLabelString(){
		return this.strategyLabelString;
	}

	@Override
	public void initStrategy() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Line2D[] getStrategyLines() {
		return defenceLines;
	}
}