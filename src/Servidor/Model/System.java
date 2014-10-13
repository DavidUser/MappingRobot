package Servidor.Model;

public class System {
	public Analyser analyser;
	public Map map;
	public boolean logged;

	public System() {
		map = null;
		logged = false;
		throw new UnsupportedOperationException();
	}

	public void login(User user) {
		throw new UnsupportedOperationException();
	}

	public void setMapPrecision(int precision) {
		if (map != null)
			System.err.println("set a precision of map, erase all content of map");
		map = new Map(1, 1, precision);
		//TODO send message to microcontroller
		throw new UnsupportedOperationException();
	}

	public void setMapArea(int height, int width) {
		if (map == null) {
			map = new Map(height, width, 1);
		} else {
			map.setHeight(height);
			map.setWidth(width);
		}
		//TODO send message to microcontroller
		throw new UnsupportedOperationException();
	}

	public void stopService() {
		//TODO send message to microcontroller
		throw new UnsupportedOperationException();
	}

	public void startService() {
		//TODO send message to microcontroller
		throw new UnsupportedOperationException();
	}
}
