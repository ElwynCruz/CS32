class Landmark
{
public:
	Landmark(string name)
	{
		m_name = name;
	}
	virtual ~Landmark()
	{

	}
	virtual string color() const
	{
		return "yellow";
	}
	string name() const
	{
		return m_name;
	}
	virtual string icon() const = 0;


private:
	string m_name;
};

class Hotel : public Landmark
{
public:
	Hotel(string name)
		:Landmark(name)
	{
	}
	virtual ~Hotel()
	{
		cout << "Destroying the hotel " << name() << "." << endl;
	}
	
	virtual string icon() const
	{
		return "bed";
	}

};

class Restaurant : public Landmark
{
public:
	Restaurant(string name, int capacity)
		:Landmark(name)
	{
		if (capacity < 40)
			m_icon = "small knife/fork icon";
		else
			m_icon = "large knife/fork icon";
	}
	virtual ~Restaurant()
	{
		cout << "Destroying the restaurant " << name() << "." << endl;
	}
	
	virtual string icon() const
	{
		return m_icon;
	}

private:
	string m_icon;
};


class Hospital : public Landmark
{
public:
	Hospital(string name)
		:Landmark(name)
	{

	}

	virtual ~Hospital()
	{
		cout << "Destroying the hospital " << name() << "." << endl;
	}

	virtual string color() const
	{
		return "blue";
	}
	virtual string icon() const
	{
		return "H";
	}
};
