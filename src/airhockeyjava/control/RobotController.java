package airhockeyjava.control;

import airhockeyjava.physical.Mallet;
import airhockeyjava.util.Vector2;

/**
 * Mechanical mallet controller, which is responsible for controlling 
 * the mallet, outputting appropriate control signals.
 * 
 * @author Joshua Segeren
 *
 */
public class RobotController implements IController {

	private final PathPlanner pathPlanner;
	private final Mallet mallet;

	RobotController(Mallet mallet) {
		this.mallet = mallet;
		pathPlanner = new PathPlanner(mallet);
	}

	@Override
	public void controlMallet(Vector2 targetPosition, float deltaTime) {
		// TODO Auto-generated method stub

	}
}