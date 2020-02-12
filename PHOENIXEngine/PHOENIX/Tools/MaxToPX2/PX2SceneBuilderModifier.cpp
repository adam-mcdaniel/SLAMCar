// PX2SceneBuilderModifier.cpp

#include "PX2SceneBuilder.hpp"

//----------------------------------------------------------------------------
void SceneBuilder::ApplyModifiers()
{
	// �����ڵ�ʹ���ռ����޸�������ǰΨһ֧�ֵ��޸�����skin��

	std::vector<ModifierInfo*>::iterator itI = mModifierList.begin();
	for (; itI!=mModifierList.end(); itI++)
	{
		ModifierInfo *info = *itI;
		
		std::vector<Modifier*>::iterator itJ = info->Modifiers.begin();
		for (; itJ!=info->Modifiers.end(); itJ++)
		{
			Modifier *modifier = *itJ;
			Class_ID id = modifier->ClassID();
			if (id == SKIN_CLASSID)
			{
				ProcessSkin(info->Node, modifier);
			}
			else if (id == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
				ProcessPhysique(info->Node, modifier);
			}
		}
	}
}
//----------------------------------------------------------------------------
void SceneBuilder::CollectModifiers (INode *node, 
								 std::vector<Modifier*> &modifiers)
{
	// ȷ������ڵ��Ƿ����޸���������У��洢������
	//
	// node:
	//		Max�����нڵ�ָ�롣
	// modifiers:
	//		�洢�����޸���ָ�롣

	Object *obj = node->GetObjectRef();
	if (!obj)
		return;

	while (obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* derObj = static_cast<IDerivedObject*>(obj);

		int modStackIndex = 0;
		while (modStackIndex < derObj->NumModifiers())
		{
			Modifier *mod = derObj->GetModifier(modStackIndex);
			if (mod)
			{
				modifiers.push_back(mod);
			}

			modStackIndex++;
		}

		obj = derObj->GetObjRef();
	}
}
//----------------------------------------------------------------------------
bool SceneBuilder::IsNodeHasModifierSkin (INode *node)
{
	std::vector<Modifier*> modifiers;
	CollectModifiers(node, modifiers);

	for (int i=0; i<(int)modifiers.size(); i++)
	{
		Modifier *modifier = modifiers[i];
		Class_ID id = modifier->ClassID();
		if (id == SKIN_CLASSID ||
			id == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
int SceneBuilder::GetNumModifiers (INode *node)
{
	Object *obj = node->GetObjectRef();
	if (!obj)
		return 0;

	std::vector<Modifier*> modifiers;

	while (obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* derObj = static_cast<IDerivedObject*>(obj);

		int modStackIndex = 0;
		while (modStackIndex < derObj->NumModifiers())
		{
			Modifier *mod = derObj->GetModifier(modStackIndex);
			if (mod)
			{
				modifiers.push_back(mod);
			}

			modStackIndex++;
		}

		obj = derObj->GetObjRef();
	}

	return (int)modifiers.size();
}
//----------------------------------------------------------------------------