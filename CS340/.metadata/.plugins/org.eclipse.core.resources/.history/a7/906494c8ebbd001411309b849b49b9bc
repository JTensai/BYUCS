package client.controller.turntracker;

import java.io.IOException;
import java.util.Collections;
import java.util.List;
import java.util.Observable;
import java.util.Observer;

import shared.CatanModel;
import shared.Player;
import shared.definitions.CatanColor;
import client.CatanGame;
import client.comm.IServerProxy;
import client.view.base.*;
import client.view.turntracker.ITurnTrackerView;


/**
 * Implementation for the turn tracker controller
 */
public class TurnTrackerController extends Controller implements ITurnTrackerController, Observer 
{

	private CatanModel catanModel;
	private IServerProxy serverProxy;
	private int numPlayers;
	
	public TurnTrackerController(ITurnTrackerView view) 
	{
		
		super(view);
		this.serverProxy = serverProxy;
		numPlayers = 0;
		initFromModel();
	}
	
	public void updateFromModel()
	{
		List<Player> players = null;
		for (int i = 0; i < catanModel.getPlayers().length; i++)
			players.add(catanModel.getPlayers()[i]);
		players.removeAll(Collections.singleton(null));
			
		if (numPlayers < players.size()) 
		{
			for (int i = numPlayers; i < players.size(); i++) {
				getView().initializePlayer(players.get(i).getPlayerIndex(), 
						players.get(i).getName(), 
						players.get(i).getColor());
			}
			
			numPlayers = players.size();
		}
		
		for(Player p : players){
			getView().updatePlayer(p.getPlayerIndex(), p.getVictoryPoints(), isPlayersTurn(p), ifLargestArmy(p), ifLongestRoad(p));									
		}

	}
	
	private boolean isPlayersTurn(Player p){
		if(p.getPlayerIndex() == catanModel.getTurnTracker().getCurrentTurn())
			return true;
		return false;
	}

	private boolean ifLargestArmy(Player p){
		if(p.getPlayerIndex() == catanModel.getTurnTracker().getLargestArmy())
			return true;
		return false;
	}
	
	private boolean ifLongestRoad(Player p){
		if(p.getPlayerIndex() == catanModel.getTurnTracker().getLongestRoad())
			return true;
		return false;
	}
	
	@Override
	public ITurnTrackerView getView() {
		
		return (ITurnTrackerView)super.getView();
	}

	@Override
	public void endTurn() 
	{
		try {
			serverProxy.movesFinishTurn(catanModel.getTurnTracker().getCurrentTurn());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private void initFromModel() {
		//<temp>
		getView().setLocalPlayerColor(CatanColor.RED);
		//</temp>
	}

	@Override
	public void update(Observable obs, Object obj) {
		if (obs instanceof CatanGame) {
			catanModel = ((CatanGame) obs).getModel();
			updateFromModel();
		}
	}

}

