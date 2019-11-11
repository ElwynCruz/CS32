void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m == nullptr)
		return;

	if (m->menuItems() == nullptr)
		return;

	for (int i = 0; i < (*m->menuItems()).size(); i++)
	{
		cout << path + (*m->menuItems())[i]->name() << endl;
		listAll((*m->menuItems())[i], path + (*m->menuItems())[i]->name() + "/");
	}
	

}