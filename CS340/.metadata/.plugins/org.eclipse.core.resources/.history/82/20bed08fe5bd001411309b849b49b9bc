package client.controller.devcards;

import java.util.Observable;
import java.util.Observer;

import javax.swing.JToggleButton;

import shared.CanCan;
import shared.CatanModel;
import shared.DevCardList;
import shared.Player;
import shared.ResourceList;
import shared.TurnTracker;
import shared.definitions.DevCardType;
import shared.definitions.ResourceType;
import client.CatanGame;
import client.view.base.*;
import client.view.devcards.IBuyDevCardView;
import client.view.devcards.IPlayDevCardView;
import client.view.resources.ResourceBarElement;


/**
 * "Dev card" controller implementation
 */
public class DevCardController extends Controller implements IDevCardController, Observer {

	private IBuyDevCardView buyCardView;
	private IAction soldierAction;
	private IAction roadAction;
	
	// variable added by Jordan
	private CatanModel model;
	private Player thisPlayer;
	private TurnTracker turn;
	private DevCardList deck;
	private ResourceList bank;
	// end variables
	
	/**
	 * DevCardController constructor
	 * 
	 * @param view "Play dev card" view
	 * @param buyCardView "Buy dev card" view
	 * @param soldierAction Action to be executed when the user plays a soldier card.  It calls "mapController.playSoldierCard()".
	 * @param roadAction Action to be executed when the user plays a road building card.  It calls "mapController.playRoadBuildingCard()".
	 */
	public DevCardController(IPlayDevCardView view, IBuyDevCardView buyCardView, 
								IAction soldierAction, IAction roadAction) {

		super(view);
		
		this.buyCardView = buyCardView;
		this.soldierAction = soldierAction;
		this.roadAction = roadAction;
	}

	public IPlayDevCardView getPlayCardView() {
		return (IPlayDevCardView)super.getView();
	}

	public IBuyDevCardView getBuyCardView() {
		return buyCardView;
	}

	@Override
	public void startBuyCard() {
		getBuyCardView().showModal();
	}

	@Override
	public void cancelBuyCard() {		
		getBuyCardView().closeModal();
	}

	@Override
	public void buyCard() {		
		getBuyCardView().closeModal();
	}

	@Override
	public void startPlayCard() {		
		getPlayCardView().showModal();
	}

	@Override
	public void cancelPlayCard() {
		getPlayCardView().closeModal();
	}

	// Implemented -Jordan
	@Override
	public void playMonopolyCard(ResourceType resource) {
		int count = 0;
		// collect all the resources from other players
		for(Player p : model.getPlayers()){
			if (p.getPlayerID() != thisPlayer.getPlayerID()){
				count += p.getResources().getResource(resource);
				p.getResources().setResource(resource, 0);
			}
		}
		// add resources to thisPlayer
		thisPlayer.getResources().setResource(resource, count);
		// remove card from player
		thisPlayer.getOldDevCards().setMonopoly(thisPlayer.getOldDevCards().getMonopoly() - 1);
	}

	// Implemented -Jordan
	@Override
	public void playMonumentCard() {
		// add 1 to players VictoryPoints
		thisPlayer.setVictoryPoints(thisPlayer.getVictoryPoints()+1);
		// remove card from player
		thisPlayer.getOldDevCards().setMonument(thisPlayer.getOldDevCards().getMonument() - 1);
	}

	// Partially implemented? -Jordan
	@Override
	public void playRoadBuildCard() {
		
		roadAction.execute();
		// remove card from player
		thisPlayer.getOldDevCards().setRoadBuilding(thisPlayer.getOldDevCards().getRoadBuilding() - 1);
	}

	// Partially implemented? -Jordan
	@Override
	public void playSoldierCard() {
		
		soldierAction.execute();
		// remove card from player
		thisPlayer.getOldDevCards().setSoldier(thisPlayer.getOldDevCards().getSoldier() - 1);
	}

	// Implemented -Jordan
	@Override
	public void playYearOfPlentyCard(ResourceType resource1, ResourceType resource2) {
		// add resources to thisPlayer
		thisPlayer.getResources().setResource(resource1, thisPlayer.getResources().getResource(resource1) + 1);
		thisPlayer.getResources().setResource(resource2, thisPlayer.getResources().getResource(resource2) + 1);
		// remove resources from bank
		bank.setResource(resource1, bank.getResource(resource1) - 1);
		bank.setResource(resource2, bank.getResource(resource2) - 1);
		// remove card from player
		thisPlayer.getOldDevCards().setYearOfPlenty(thisPlayer.getOldDevCards().getYearOfPlenty() - 1);
	}

	@Override
	public void update(Observable o, Object arg) {
		if (o instanceof CatanGame) {
			model = ((CatanGame) o).getModel();
			thisPlayer = model.getPlayers()[((CatanGame) o).getPlayerInfo().getId()];
			turn = model.getTurnTracker();
			deck = model.getDeck();
			bank = model.getBank();
			
			// update counts
			getPlayCardView().setCardAmount(DevCardType.MONOPOLY, thisPlayer.getOldDevCards().getMonopoly() + thisPlayer.getNewDevCards().getMonopoly());
			getPlayCardView().setCardAmount(DevCardType.MONUMENT, thisPlayer.getOldDevCards().getMonument() + thisPlayer.getNewDevCards().getMonument());
			getPlayCardView().setCardAmount(DevCardType.ROAD_BUILD, thisPlayer.getOldDevCards().getRoadBuilding() + thisPlayer.getNewDevCards().getRoadBuilding());
			getPlayCardView().setCardAmount(DevCardType.SOLDIER, thisPlayer.getOldDevCards().getSoldier() + thisPlayer.getNewDevCards().getSoldier());
			getPlayCardView().setCardAmount(DevCardType.YEAR_OF_PLENTY, thisPlayer.getOldDevCards().getYearOfPlenty() + thisPlayer.getNewDevCards().getYearOfPlenty());

			
			// enable based on OldDevCardList
			getPlayCardView().setCardEnabled(DevCardType.MONOPOLY, CanCan.canUseMonopoly(thisPlayer, turn));
			getPlayCardView().setCardEnabled(DevCardType.MONUMENT, CanCan.canUseMonument(thisPlayer, turn));
			getPlayCardView().setCardEnabled(DevCardType.ROAD_BUILD, CanCan.canUseRoadBuilder(thisPlayer, turn) && thisPlayer.getRoads() >= 1);
			getPlayCardView().setCardEnabled(DevCardType.SOLDIER, CanCan.canUseSoldier(thisPlayer, turn));
			getPlayCardView().setCardEnabled(DevCardType.YEAR_OF_PLENTY, CanCan.canUseYearOfPlenty(thisPlayer, turn));		
			
			
		}
	}

}

