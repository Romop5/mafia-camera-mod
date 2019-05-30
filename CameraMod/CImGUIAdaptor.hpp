struct IDirect3DDevice9;

class CImGUIAdaptor
{
	public:
		void Initialize(IDirect3DDevice9* device);
		void Render();
		void CleanUP();
	private:
};

