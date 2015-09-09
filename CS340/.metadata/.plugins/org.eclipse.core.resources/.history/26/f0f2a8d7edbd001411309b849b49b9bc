package client.controller.resources;

import java.util.*;

import shared.CanCan;
import shared.CatanModel;
import shared.DevCardList;
import shared.Player;
import shared.TurnTracker;
import client.CatanGame;
import client.view.base.*;
import client.view.resources.IResourceBarView;
import client.view.resources.ResourceBarElement;

/**
 * Implementation for the resource bar controller
 */
public class ResourceBarController extends Controller implements
		IResourceBarController, Observer {

	private Map<ResourceBarElement, IAction> elementActions;

	public ResourceBarController(IResourceBarView view, CatanGame catanGame) {

		super(view);
		catanGame.addObserver(this);
		elementActions = new HashMap<ResourceBarElement, IAction>();
	}

	@Override
	public IResourceBarView getView() {
		return (IResourceBarView) super.getView();
	}

	/**
	 * Sets the action to be executed when the specified resource bar element is
	 * clicked by the user
	 * 
	 * @param element
	 *            The resource bar element with which the action is associated
	 * @param action
	 *            The action to be executed
	 */
	public void setElementAction(ResourceBarElement element, IAction action) {

		elementActions.put(element, action);
	}

	// This is set up so that the action for ROAD is to have the gui build the window that allows the player to place the road
	// All I have to do is manage whether the button is enabled or disabled.
	@Override
	public void buildRoad() {
		executeElementAction(ResourceBarElement.ROAD);
	}

	@Override
	public void buildSettlement() {
		executeElementAction(ResourceBarElement.SETTLEMENT);
	}

	@Override
	public void buildCity() {
		executeElementAction(ResourceBarElement.CITY);
	}

	@Override
	public void buyCard() {
		executeElementAction(ResourceBarElement.BUY_CARD);
	}

	@Override
	public void playCard() {
		executeElementAction(ResourceBarElement.PLAY_CARD);
	}

	private void executeElementAction(ResourceBarElement element) {

		if (elementActions.containsKey(element)) {

			IAction action = elementActions.get(element);
			action.execute();
		}
	}

	@Override
	public void update(Observable o, Object arg) {
		if (o instanceof CatanGame) {
			CatanModel model = ((CatanGame) o).getModel();
			//Player thisPlayer = model.getPlayers()[((CatanGame) o).getPlayerInfo().getPlayerIndex()];
			Player thisPlayer = model.getPlayers()[0];
			TurnTracker turn = model.getTurnTracker();
			DevCardList deck = model.getDeck();

			// buildRoad check
			if (CanCan.canBuyRoad(thisPlayer, turn)) {
				getView().setElementEnabled(ResourceBarElement.ROAD, true);
			} else {
				getView().setElementEnabled(ResourceBarElement.ROAD, false);
			}
			
			// buildSettlement check
			if (CanCan.canBuySettlement(thisPlayer, turn)) {
				getView().setElementEnabled(ResourceBarElement.SETTLEMENT, true);
			} else {
				getView().setElementEnabled(ResourceBarElement.SETTLEMENT, false);
			}
			
			// buildCity check
			if (CanCan.canBuyCity(thisPlayer, turn)) {
				getView().setElementEnabled(ResourceBarElement.CITY, true);
			} else {
				getView().setElementEnabled(ResourceBarElement.CITY, false);
			}
			
			// buyCard check
			if (CanCan.canBuyDevCard(thisPlayer, deck, turn)) {
				getView().setElementEnabled(ResourceBarElement.BUY_CARD, true);
			} else {
				getView().setElementEnabled(ResourceBarElement.BUY_CARD, false);
			}
			
			// playCard check
			if (thisPlayer.getOldDevCards().getTotal() > 0) {
				getView().setElementEnabled(ResourceBarElement.PLAY_CARD, true);
			} else {
				getView().setElementEnabled(ResourceBarElement.PLAY_CARD, true);
				// apparently you can always click on play dev card, i'll remove this whole section if true
				//getView().setElementEnabled(ResourceBarElement.PLAY_CARD, false);
			}

		}

	}

}
