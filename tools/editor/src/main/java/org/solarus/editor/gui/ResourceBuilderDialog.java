package org.solarus.editor.gui;

import org.solarus.editor.QuestEditorException;
import org.solarus.editor.ResourceType;

/**
 * Dialog shown when we want to create a new map in the quest
 */
public class ResourceBuilderDialog extends OkCancelDialog {
	private static final long serialVersionUID = 1L;

	private ResourceBuilderComponent resourceBuilder;
	
	public ResourceBuilderDialog(ResourceType resourceType) {
		super("New " + resourceType.getName().toLowerCase(), false);
		
		resourceBuilder = new ResourceBuilderComponent(resourceType);
		setComponent(resourceBuilder);
		
	}
	
	public String getId() {
		return resourceBuilder.getId();
	}
	public String getFriendlyName() {
		return resourceBuilder.getFriendlyName();
	}
		
	@Override
	protected void applyModifications() throws QuestEditorException {
		
	}
	
}
